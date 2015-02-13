#include "ub_EventRecord.h"

using namespace gov::fnal::uboone::datatypes;


void ub_EventRecord::addFragment(raw_fragment_data_t& fragment) throw(datatypes_exception)
{
    artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* fragment.begin());

    if(artdaq_fragment_header::num_words()<=fragment.size())
        throw datatypes_exception("Invalid fragment: fragment is too short.");

    ub_CrateHeader_v6 const & crate_header= ub_CrateHeader_v6::getHeaderFromFragment(fragment);

    if(crate_header.crate_type == 2)
    {
        _pmt_seb_map.emplace(crate_header.crate_number,std::make_tuple(
                                 std::move(fragment),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<pmt_crate_data_t>(nullptr)));

        raw_fragment_data_t& tpm_fragment=std::get<0>(_pmt_seb_map[crate_header.crate_number]);
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_pmt_seb_map[crate_header.crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<pmt_crate_data_t>(*std::get<1>(_pmt_seb_map[crate_header.crate_number]));
        auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
        crate_data->crateHeader().swap(header);
        std::get<2>(_pmt_seb_map[crate_header.crate_number]).swap(crate_data);
    }
    else
    {
        _tpc_seb_map.emplace(crate_header.crate_number,std::make_tuple(
                                 std::move(fragment),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<tpc_crate_data_t>(nullptr)));

        raw_fragment_data_t& tpm_fragment=std::get<0>(_tpc_seb_map[crate_header.crate_number]);
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_tpc_seb_map[crate_header.crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<tpc_crate_data_t>(*std::get<1>(_tpc_seb_map[crate_header.crate_number]));
        auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
        std::get<2>(_tpc_seb_map[crate_header.crate_number]).swap(crate_data);
    }

    getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size);
    getGlobalHeader().setNumberOfSEBs((uint8_t)(_tpc_seb_map.size() + _pmt_seb_map.size()));
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

bool ub_EventRecord::compare(const ub_EventRecord& event_record, bool do_rethrow=false) const throw(datatypes_exception)
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
        std::cerr << "Unknown exception.";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception.");
        else
            return false;
    }
    return true;
}
