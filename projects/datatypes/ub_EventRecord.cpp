#include "ub_EventRecord.h"

using namespace gov::fnal::uboone::datatypes;


ub_EventRecord::ub_EventRecord()
    :_bookkeeping_header(),
     _bookkeeping_trailer(),
     _global_header(),
     _tpc_seb_map(),
     _pmt_seb_map(),
     _trigger_data(),
     _gps_data(),
     _beam_record() {
}

ub_EventRecord::~ub_EventRecord()
{
    for(auto& pmt : _pmt_seb_map)
            std::get<0>(pmt.second).clear();
    _pmt_seb_map.clear();
    
    for(auto& tpc : _tpc_seb_map)
            std::get<0>(tpc.second).clear();
    _tpc_seb_map.clear();
}

void ub_EventRecord::setGlobalHeader (global_header_t & header) noexcept {
    _global_header = header;
}

global_header_t& ub_EventRecord::getGlobalHeader() noexcept {
    return _global_header;
}
void ub_EventRecord::setTriggerData (ub_TriggerData const& trigger_data) noexcept {
    _trigger_data = trigger_data;
}
void ub_EventRecord::setGPSData(ub_GPS const& gps_data) noexcept {
    _gps_data = gps_data;
}
void ub_EventRecord::setBeamRecord(ub_BeamRecord const& beam_record) noexcept {
    _beam_record=beam_record;
}
ub_GPS const& ub_EventRecord::GPSData() const noexcept {
    return _gps_data;
}
ub_TriggerData const& ub_EventRecord::triggerData()const noexcept {
    return _trigger_data;
}
ub_BeamRecord const& ub_EventRecord::beamRecord()const noexcept {
    return _beam_record;
}
ub_BeamRecord& ub_EventRecord::beamRecord() noexcept {return _beam_record;}

std::size_t ub_EventRecord::getFragmentCount() const noexcept
{
  return _pmt_seb_map.size()+_tpc_seb_map.size();
}

void ub_EventRecord::addFragment(raw_fragment_data_t& fragment) throw(datatypes_exception)
{
    ub_CrateHeader_v6 const & crate_header= ub_CrateHeader_v6::getHeaderFromFragment(ub_RawData(fragment.begin(),fragment.end()));

    int crate_number {crate_header.crate_number};
    uint8_t crate_type {crate_header.crate_type};

    
    if(crate_type == 2)
    {
        _pmt_seb_map.emplace(crate_number,std::make_tuple(
                                  raw_fragment_data_t(),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<pmt_crate_data_t>(nullptr)));

        std::get<0>(_pmt_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_pmt_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        ub_CrateHeader_v6 const & crate_header= ub_CrateHeader_v6::getHeaderFromFragment(data);

        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_pmt_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<pmt_crate_data_t>(*std::get<1>(_pmt_seb_map[crate_number]));
        auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
        crate_data->crateHeader().swap(header);
        std::get<2>(_pmt_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);
            
        getGlobalHeader().setSeconds(header->gps_time.second);
        getGlobalHeader().setMicroSeconds(header->gps_time.micro);
        getGlobalHeader().setNanoSeconds(header->gps_time.nano);        
    }
    else
    {
        _tpc_seb_map.emplace(crate_number,std::make_tuple(
                                 raw_fragment_data_t(),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<tpc_crate_data_t>(nullptr)));

        std::get<0>(_tpc_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_tpc_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        ub_CrateHeader_v6 const & crate_header= ub_CrateHeader_v6::getHeaderFromFragment(data);

        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_tpc_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<tpc_crate_data_t>(*std::get<1>(_tpc_seb_map[crate_number]));
        auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
        std::get<2>(_tpc_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);
        
    }

    getGlobalHeader().setNumberOfSEBs((uint8_t)(_tpc_seb_map.size() + _pmt_seb_map.size()));    
    updateDTHeader();
}

const ub_EventRecord::tpc_map_t ub_EventRecord::getTPCSEBMap() const throw(datatypes_exception)
{
    tpc_map_t retMap;
    for(auto& tpc : _tpc_seb_map)
        retMap.emplace(tpc.first,*std::get<2>(tpc.second));
    return retMap;
}
const ub_EventRecord::pmt_map_t ub_EventRecord::getPMTSEBMap() const throw(datatypes_exception)
{
    pmt_map_t retMap;
    for(auto& pmt : _pmt_seb_map)
        retMap.emplace(pmt.first,*std::get<2>(pmt.second));
    return retMap;
}


void ub_EventRecord::getFragments(fragment_references_t& fragments) const throw(datatypes_exception)
{
    for(auto& tpc : _tpc_seb_map)
        fragments.emplace_back(&std::get<0>(tpc.second));
    for(auto& pmt : _pmt_seb_map)
        fragments.emplace_back(&std::get<0>(pmt.second));
}

void ub_EventRecord::markAsIncompleteEvent() noexcept
{
    _bookkeeping_header.is_event_complete=false;
    _global_header.markIncomplete();
}


void ub_EventRecord::updateDTHeader() throw (datatypes_exception)
{
    try
    {
        fragment_references_t fragments;
        getFragments(fragments);

        _bookkeeping_header.event_fragment_count=fragments.size();

        _bookkeeping_header.raw_event_fragments_wordcount=std::accumulate(
        fragments.begin(),fragments.end(),0u,[](auto total, auto const& fragment) {
            return total+fragment->size()*sizeof(fragment_value_type_t);
        });

        _bookkeeping_header.event_global_header_word_offset=
            ub_event_header_wordcount+
            _bookkeeping_header.raw_event_fragments_wordcount+
            _bookkeeping_header.event_fragment_count*ub_size_t_wordcount;

        _bookkeeping_header.total_event_wordcount=_bookkeeping_header.event_global_header_word_offset
                //+ ADD GLOBAL HEADER SIZES
                +ub_event_trailer_wordcount;

        _bookkeeping_header.event_format_version= gov::fnal::uboone::datatypes::constants::VERSION;

        _bookkeeping_header.is_event_complete=true;
    } catch(datatypes_exception &ex) {
        throw;
    } catch(...) {
        throw datatypes_exception("Unknown exception in ub_EventRecord::updateDTHeader()");
    }
}

bool ub_EventRecord::compare(ub_EventRecord const& event_record, bool do_rethrow=false) const throw(datatypes_exception)
{
    try
    {
        //compare headers
        //_global_header.compare(event_record._global_header);

        //compare binary data
        if(_tpc_seb_map.size() !=event_record._tpc_seb_map.size())
            throw datatypes_exception(make_compare_message("_tpc_seb_map", "size", _tpc_seb_map.size(),event_record._tpc_seb_map.size()));

        if(_pmt_seb_map.size() !=event_record._pmt_seb_map.size())
            throw datatypes_exception( make_compare_message("_pmt_seb_map", "size", _pmt_seb_map.size(),event_record._pmt_seb_map.size()));

        for(tpc_seb_map_t::value_type const& tpc : _tpc_seb_map)
        {
            auto const& right=tpc.second;
            auto const& left=event_record._tpc_seb_map.at(tpc.first);

            auto const& raw_data_left= std::get<0>(left);
            auto const& raw_data_right= std::get<0>(right);
            if(raw_data_right.size()!=raw_data_left.size())
                throw datatypes_exception(make_compare_message("_tpc_seb_map.raw_fragment_data", "size", raw_data_right.size() ,raw_data_left.size()));

            auto mismatching_data = std::mismatch(raw_data_right.begin(), raw_data_right.end(), raw_data_left.begin());
            auto data_ends =std::make_pair(raw_data_right.end(),raw_data_left.end());
            if(mismatching_data!=data_ends)
                throw datatypes_exception("Error: _tpc_seb_map.raw_fragment_data are not matching");

            auto const& ptr_data_left= std::get<1>(left);
            auto const& ptr_data_right= std::get<1>(right);
            if(!ptr_data_right->compare(*ptr_data_left))
                throw datatypes_exception(make_compare_message("_tpc_seb_map", "raw_data", ptr_data_right.get() ,ptr_data_left.get()));

            auto const& crate_left= std::get<2>(left);
            auto const& crate_right= std::get<2>(right);
            if(!crate_right->compare(*crate_left))
                throw datatypes_exception(make_compare_message("_tpc_seb_map", "crate_left", ptr_data_right.get() ,ptr_data_left.get()));
        }
    } catch (std::out_of_range &e) {
        std::cerr << "Error: _tpc_seb_map has different crates; exception" << e.what();
        if(do_rethrow)
            throw datatypes_exception(std::string( "Error: _tpc_seb_map has different crates; exception").append(e.what()));
        else
            return false;
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_EventRecord::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_EventRecord::compare()");
        else
            return false;
    }
    return true;
}


std::string ub_EventRecord::debugInfo()const noexcept {
    std::ostringstream os;
    auto tpcs = getTPCSEBMap();
    auto pmts = getPMTSEBMap();

    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n TPC fragment count=" << tpcs.size();
    os << "\n PMT fragment count=" << pmts.size() << std::endl;
    os << _global_header.debugInfo() << std::endl;
    os << _trigger_data.debugInfo() << std::endl;
    os << _gps_data.debugInfo() << std::endl;
    os << _beam_record.debugInfo() << std::endl;

    os << "\nTPC fragments";
    for(auto const& tpc: tpcs)
        os << "\n" << tpc.second.debugInfo();

    os << "\nPMT fragments";
    for(auto const& pmt: pmts)
        os << "\n" << pmt.second.debugInfo();

    return os.str();
}
