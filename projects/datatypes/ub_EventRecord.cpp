#include "ub_EventRecord.h"
#include <memory>

#ifdef __clang__
// Only the most recent GCC compiler has this defined in the standard library. 
// I wish you guys wouldn't always use bleeding-edge syntactic sugar.  --Nathaniel
namespace std {
  template<typename T, typename ...Args>
  std::unique_ptr<T> make_unique( Args&& ...args )
  {
      return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
  }
}
#endif

using namespace gov::fnal::uboone::datatypes;


ub_EventRecord::ub_EventRecord()
    :_bookkeeping_header(),
     _bookkeeping_trailer(),
     _global_header(),
     _trigger_data(),
     _trigger_counter(),
     _tpc_seb_map(),
     _pmt_seb_map(),
     _trigger_seb_map(),
     _laser_seb_map(),
     _beam_record(),
     _swtrigger_output_vector()
{
}

void ub_EventRecord::setCrateSerializationMask(uint16_t mask) throw (datatypes_exception)
{
    _crate_serialization_mask.store(mask);
    updateDTHeader();
}

int ub_EventRecord::eventRecordVersion = constants::DATATYPES_VERSION;

int ub_EventRecord::getEventRecordVersion() noexcept
{
    return eventRecordVersion;
}

ub_EventRecord::~ub_EventRecord()
{
    for(auto& pmt : _pmt_seb_map)
            std::get<0>(pmt.second).clear();
    _pmt_seb_map.clear();
    
    for(auto& tpc : _tpc_seb_map)
            std::get<0>(tpc.second).clear();
    _tpc_seb_map.clear();

    for(auto& trg : _trigger_seb_map)
            std::get<0>(trg.second).clear();
    _trigger_seb_map.clear();

    _laser_seb_map.clear();
}

void ub_EventRecord::setGlobalHeader (global_header_t const& header) noexcept {
    _global_header = header;
}

global_header_t& ub_EventRecord::getGlobalHeader() noexcept {
    return _global_header;
}

trigger_counter_t const& ub_EventRecord::getTriggerCounter() noexcept {
  return _trigger_counter;
}
void ub_EventRecord::resetTriggerCounter() noexcept {
  _trigger_counter.reset();
}
void ub_EventRecord::setTriggerCounter( trigger_counter_t const& tc) noexcept {
  _trigger_counter = tc;
}

trig_data_t const& ub_EventRecord::getTriggerData() noexcept {
  return _trigger_data;
}

bool ub_EventRecord::passesSoftwarePrescale( std::map< uint16_t, float> const& ps, uint16_t trig_value, double random ) noexcept{
  return _trigger_counter.prescalePass(ps, trig_value, random);
}


void ub_EventRecord::addSWTriggerOutput( ub_FEMBeamTriggerOutput const& to) noexcept{
  _swtrigger_output_vector.emplace_back(to);
}
std::vector<ub_FEMBeamTriggerOutput> const& ub_EventRecord::getSWTriggerOutputVector() noexcept{
  return _swtrigger_output_vector;
}

void ub_EventRecord::setGPSTime(ub_GPS_Time const& gps_time) noexcept{
    _global_header.setGPSTime(gps_time);
}

void ub_EventRecord::setGPSEVTTime(ub_GPS_Time const& gps_time) noexcept{
    _global_header.setGPSEVTTime(gps_time);
}

void ub_EventRecord::setTriggerBoardClock(ub_TriggerBoardClock const& trigger_board_time) noexcept{
    _global_header.setTriggerBoardClock(trigger_board_time);
}

void ub_EventRecord::setTriggerBoardEVTClock(ub_TriggerBoardClock const& trigger_board_time) noexcept{
    _global_header.setTriggerBoardEVTClock(trigger_board_time);
}

void ub_EventRecord::setLocalHostTime(ub_LocalHostTime const& localhost_time) noexcept{
    _global_header.setLocalHostTime(localhost_time);
}

void ub_EventRecord::setBeamRecord(ub_BeamRecord const& beam_record) noexcept {
    _beam_record=beam_record;
}

ub_GPS_Time const& ub_EventRecord::GPSTime() const noexcept {
    return _global_header.getGPSTime();
}

ub_GPS_Time const& ub_EventRecord::GPSEVTTime() const noexcept {
    return _global_header.getGPSEVTTime();
}

ub_TriggerBoardClock const& ub_EventRecord::TriggerBoardClock() const noexcept{
    return _global_header.getTriggerBoardClock();
}

ub_TriggerBoardClock const& ub_EventRecord::TriggerBoardEVTClock() const noexcept{
    return _global_header.getTriggerBoardEVTClock();
}

ub_LocalHostTime const& ub_EventRecord::LocalHostTime() const noexcept{
    return _global_header.getLocalHostTime();
}
    
ub_BeamRecord const& ub_EventRecord::beamRecord()const noexcept {
    return _beam_record;
}

ub_BeamRecord& ub_EventRecord::beamRecord() noexcept {
return _beam_record;
}

std::size_t ub_EventRecord::getFragmentCount() const noexcept{
  return _pmt_seb_map.size()+_tpc_seb_map.size()+_trigger_seb_map.size();
}

void ub_EventRecord::addFragment(raw_fragment_data_t& fragment) throw(datatypes_exception,data_size_exception)
{
    try {

    crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(ub_RawData(fragment.begin(),fragment.end()));

    int crate_number {crate_header.crate_number};
    uint8_t crate_type {crate_header.crate_type};

    
    if(crate_type == SystemDesignator::TRIGGER_SYSTEM)
      {
        _trigger_seb_map.emplace( crate_number,std::make_tuple(raw_fragment_data_t(),
                                                               std::unique_ptr<ub_RawData>(nullptr),
                                                               std::unique_ptr<trig_crate_data_t>(nullptr)));
      
        std::get<0>(_trigger_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_trigger_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);
        
	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_trigger_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<trig_crate_data_t>(*std::get<1>(_trigger_seb_map[crate_number]));//do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);
        crate_data->crateHeader().swap(header); //use crate header created by a seb   
        std::get<2>(_trigger_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);
        if(crate_header.local_host_time.wasSet())
          getGlobalHeader().setLocalHostTime(crate_header.local_host_time);


	auto const trigMaptmp = getTRIGSEBMap();
	if (trigMaptmp.size()==1)
	  {
	    auto const hdr2 = trigMaptmp.begin()->second.getTriggerHeader();
	    int  frame2  = hdr2.getFrame();
	    int  sample2 = hdr2.get2MHzSampleNumber();
	    int  div2    = hdr2.get16MHzRemainderNumber();
	    //	    std::cout << "\n ub_EVENT_RECORD___TMP:: Trigger Clock OF THIS EVENT (" << crate_header.event_number << "): (frame2,sample2,div2) " << (int) frame2 << ", " << (int) sample2 << ", " << (int) div2 << std::endl;  	
	    getGlobalHeader().setTriggerBoardEVTClock(ub_TriggerBoardClock(frame2,sample2,div2));
	  }



	double gps_adj(0.0);
        if(crate_header.trigger_board_time.wasSet())
	  {
	    getGlobalHeader().setTriggerBoardClock(crate_header.trigger_board_time);
	    int framePPSMap = crate_header.trigger_board_time.frame;
	    int samplePPSMap = crate_header.trigger_board_time.sample;
	    int divPPSMap = crate_header.trigger_board_time.div;
	    auto const trigMap = getTRIGSEBMap();
	    if (trigMap.size()==1)
	      {
		auto const hdr = trigMap.begin()->second.getTriggerHeader();
		int  frame  = hdr.getFrame();
		int  sample = hdr.get2MHzSampleNumber();
		int  div    = hdr.get16MHzRemainderNumber();
		//		std::cout << "\n ub_EVENT_RECORD___TMP:: Trigger Clock OF THIS EVENT (" << crate_header.event_number << "): (frame,sample,div) " << (int) frame << ", " << (int) sample << ", " << (int) div << std::endl;  	
		//     getGlobalHeader().setTriggerBoardEVTClock(ub_TriggerBoardClock(frame,sample,div));
		gps_adj = ((frame-framePPSMap)*1600. + (sample-samplePPSMap)*0.5 + (div-divPPSMap)*0.00624)*1.0E-6;   // musec->sec
	      }
	  }
        if(crate_header.gps_time.wasSet())
	  {
	    getGlobalHeader().setGPSTime(crate_header.gps_time);
	    if (crate_header.trigger_board_time.wasSet())
	      {
		double gpsPPSMap = crate_header.gps_time.second + crate_header.gps_time.micro*(1.0E-6) + crate_header.gps_time.nano*(1.0E-9);
		double gps_evt = gpsPPSMap + gps_adj ;
		int musec_adj = gps_evt*1.0E6 - ((int) gps_evt)*1.0E6; // will mod 1E6 this later too ...
		// possible loss of precision in lsb's requires going back to first quantities for nsec
		int nsec_adj  = crate_header.gps_time.nano + gps_adj*1.0E9; 
		gps_sign_adjust(musec_adj,nsec_adj);
		ub_GPS_Time gps_t_adj(int(gps_evt ), musec_adj, nsec_adj) ;
		std::cout << "ub_EVENT_RECORD:: gps deltatime OF THIS EVENT (sec,micro,nano):  " << gps_t_adj.second << ", " << gps_t_adj.micro <<  ", " << gps_t_adj.nano << std::endl;
		getGlobalHeader().setGPSEVTTime(gps_t_adj);
	      }
	  }

	bool first_trig_fragment = (_trigger_seb_map.size()==1);
        _trigger_data = std::get<2>(_trigger_seb_map[crate_number])->getTriggerData();
	_trigger_counter.increment(_trigger_data, !first_trig_fragment);
      }
    else if(crate_type == SystemDesignator::PMT_SYSTEM)
    {
        _pmt_seb_map.emplace( crate_number,std::make_tuple(
                                  raw_fragment_data_t(),
                                  std::unique_ptr<ub_RawData>(nullptr),
                                  std::unique_ptr<pmt_crate_data_t>(nullptr)));

        std::get<0>(_pmt_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_pmt_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);

	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_pmt_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<pmt_crate_data_t>(*std::get<1>(_pmt_seb_map[crate_number])); //do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);        
        crate_data->crateHeader().swap(header); //use crate header created by a seb   
        std::get<2>(_pmt_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);            
    }
    else if(crate_type == SystemDesignator::TPC_SYSTEM)
    {
        _tpc_seb_map.emplace(crate_number,std::make_tuple(
                                 raw_fragment_data_t(),
                                 std::unique_ptr<ub_RawData>(nullptr),
                                 std::unique_ptr<tpc_crate_data_t>(nullptr)));

        std::get<0>(_tpc_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_tpc_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);
        
	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_tpc_seb_map[crate_number]).swap(raw_data);
       
        auto crate_data = std::make_unique<tpc_crate_data_t>(*std::get<1>(_tpc_seb_map[crate_number])); //do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);
        crate_data->crateHeader().swap(header); //use crate header created by a seb         
        std::get<2>(_tpc_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);                
    }
    else if(crate_type == SystemDesignator::LASER_SYSTEM)
    {
      raw_fragment_data_t& tpm_fragment=fragment;      
      artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
      ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());

      //this is klugey, and inefficient, but it's the best I got right now --- WK
      ub_LaserData mydata;
      mydata.setData((char*)(&(*(tpm_fragment.begin()+artdaq_header->metadata_word_count+
				 artdaq_fragment_header::num_words()))));
      _laser_seb_map.emplace(crate_number,mydata);
    }

    getGlobalHeader().setNumberOfSEBs((uint8_t)(_tpc_seb_map.size() + _pmt_seb_map.size()));    
    updateDTHeader();

    } catch(datatypes_exception &e) {
	throw ;
    }catch(std::exception &ex) {
	   std::ostringstream os;
	   os << "Caught exception in  ub_EventRecord::addFragment(). Message:";
	   os << ex.what();
	   std::cerr << os.str() << std::endl;
	   
            throw datatypes_exception(os.str());
    } catch(...) {
    	   std::ostringstream os;
	   os << "Caught unknown exception in  ub_EventRecord::addFragment()";
	   std::cerr << os.str() << std::endl;

           throw datatypes_exception(os.str());
    }    
}

void ub_EventRecord::addFragment_PMT_or_TRIG(raw_fragment_data_t& fragment) throw(datatypes_exception,data_size_exception)
{
    try {

    crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(ub_RawData(fragment.begin(),fragment.end()));

    int crate_number {crate_header.crate_number};
    uint8_t crate_type {crate_header.crate_type};

    
    if(crate_type == SystemDesignator::TRIGGER_SYSTEM)
      {
        _trigger_seb_map.emplace( crate_number,std::make_tuple(raw_fragment_data_t(),
                                                               std::unique_ptr<ub_RawData>(nullptr),
                                                               std::unique_ptr<trig_crate_data_t>(nullptr)));
      
        std::get<0>(_trigger_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_trigger_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);
        
	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_trigger_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<trig_crate_data_t>(*std::get<1>(_trigger_seb_map[crate_number]));//do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);
        crate_data->crateHeader().swap(header); //use crate header created by a seb   
        std::get<2>(_trigger_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);
        if(crate_header.local_host_time.wasSet())
          getGlobalHeader().setLocalHostTime(crate_header.local_host_time);


	auto const trigMaptmp = getTRIGSEBMap();
	if (trigMaptmp.size()==1)
	  {
	    auto const hdr2 = trigMaptmp.begin()->second.getTriggerHeader();
	    int  frame2  = hdr2.getFrame();
	    int  sample2 = hdr2.get2MHzSampleNumber();
	    int  div2    = hdr2.get16MHzRemainderNumber();
	    //	    std::cout << "\n ub_EVENT_RECORD___TMP:: Trigger Clock OF THIS EVENT (" << crate_header.event_number << "): (frame2,sample2,div2) " << (int) frame2 << ", " << (int) sample2 << ", " << (int) div2 << std::endl;  	
	    getGlobalHeader().setTriggerBoardEVTClock(ub_TriggerBoardClock(frame2,sample2,div2));
	  }



	double gps_adj(0.0);
        if(crate_header.trigger_board_time.wasSet())
	  {
	    getGlobalHeader().setTriggerBoardClock(crate_header.trigger_board_time);
	    int framePPSMap = crate_header.trigger_board_time.frame;
	    int samplePPSMap = crate_header.trigger_board_time.sample;
	    int divPPSMap = crate_header.trigger_board_time.div;
	    auto const trigMap = getTRIGSEBMap();
	    if (trigMap.size()==1)
	      {
		auto const hdr = trigMap.begin()->second.getTriggerHeader();
		int  frame  = hdr.getFrame();
		int  sample = hdr.get2MHzSampleNumber();
		int  div    = hdr.get16MHzRemainderNumber();
		//		std::cout << "\n ub_EVENT_RECORD___TMP:: Trigger Clock OF THIS EVENT (" << crate_header.event_number << "): (frame,sample,div) " << (int) frame << ", " << (int) sample << ", " << (int) div << std::endl;  	
		//     getGlobalHeader().setTriggerBoardEVTClock(ub_TriggerBoardClock(frame,sample,div));
		gps_adj = ((frame-framePPSMap)*1600. + (sample-samplePPSMap)*0.5 + (div-divPPSMap)*0.00624)*1.0E-6;   // musec->sec
	      }
	  }
        if(crate_header.gps_time.wasSet())
	  {
	    getGlobalHeader().setGPSTime(crate_header.gps_time);
	    if (crate_header.trigger_board_time.wasSet())
	      {
		double gpsPPSMap = crate_header.gps_time.second + crate_header.gps_time.micro*(1.0E-6) + crate_header.gps_time.nano*(1.0E-9);
		double gps_evt = gpsPPSMap + gps_adj ;
		int musec_adj = gps_evt*1.0E6 - ((int) gps_evt)*1.0E6; // will mod 1E6 this later too ...
		// possible loss of precision in lsb's requires going back to first quantities for nsec
		int nsec_adj  = crate_header.gps_time.nano + gps_adj*1.0E9; 
		gps_sign_adjust(musec_adj,nsec_adj);
		ub_GPS_Time gps_t_adj(int(gps_evt ), musec_adj, nsec_adj) ;
		std::cout << "ub_EVENT_RECORD:: gps deltatime OF THIS EVENT (sec,micro,nano):  " << gps_t_adj.second << ", " << gps_t_adj.micro <<  ", " << gps_t_adj.nano << std::endl;
		getGlobalHeader().setGPSEVTTime(gps_t_adj);
	      }
	  }

	bool first_trig_fragment = (_trigger_seb_map.size()==1);
        _trigger_data = std::get<2>(_trigger_seb_map[crate_number])->getTriggerData();
	_trigger_counter.increment(_trigger_data, !first_trig_fragment);
      }
    else if(crate_type == SystemDesignator::PMT_SYSTEM)
    {
        _pmt_seb_map.emplace( crate_number,std::make_tuple(
                                  raw_fragment_data_t(),
                                  std::unique_ptr<ub_RawData>(nullptr),
                                  std::unique_ptr<pmt_crate_data_t>(nullptr)));

        std::get<0>(_pmt_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_pmt_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);

	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_pmt_seb_map[crate_number]).swap(raw_data);
        auto crate_data = std::make_unique<pmt_crate_data_t>(*std::get<1>(_pmt_seb_map[crate_number])); //do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);        
        crate_data->crateHeader().swap(header); //use crate header created by a seb   
        std::get<2>(_pmt_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);            
    }
    } catch(datatypes_exception &e) {
	throw ;
    }catch(std::exception &ex) {
	   std::ostringstream os;
	   os << "Caught exception in  ub_EventRecord::addFragment(). Message:";
	   os << ex.what();
	   std::cerr << os.str() << std::endl;
	   
            throw datatypes_exception(os.str());
    } catch(...) {
    	   std::ostringstream os;
	   os << "Caught unknown exception in  ub_EventRecord::addFragment()";
	   std::cerr << os.str() << std::endl;

           throw datatypes_exception(os.str());
    }    
}

void ub_EventRecord::addFragment_TPC_or_LASER(raw_fragment_data_t& fragment) throw(datatypes_exception,data_size_exception)
{
    try {

    crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(ub_RawData(fragment.begin(),fragment.end()));

    int crate_number {crate_header.crate_number};
    uint8_t crate_type {crate_header.crate_type};

    if(crate_type == SystemDesignator::TPC_SYSTEM)
    {
        _tpc_seb_map.emplace(crate_number,std::make_tuple(
                                 raw_fragment_data_t(),
                                 std::unique_ptr<ub_RawData>(nullptr),
                                 std::unique_ptr<tpc_crate_data_t>(nullptr)));

        std::get<0>(_tpc_seb_map[crate_number]).swap(fragment);
        
        raw_fragment_data_t& tpm_fragment=std::get<0>(_tpc_seb_map[crate_number]);

        artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        crate_header_t const & crate_header= crate_header_t::getHeaderFromFragment(data);
        
	if(!crate_header.complete)
	    markAsIncompleteEvent();
	    
        auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
                        artdaq_fragment_header::num_words(),tpm_fragment.end());
        std::get<1>(_tpc_seb_map[crate_number]).swap(raw_data);
       
        auto crate_data = std::make_unique<tpc_crate_data_t>(*std::get<1>(_tpc_seb_map[crate_number])); //do not recreate crate header
        auto header=std::make_unique<crate_header_t>(crate_header);
        crate_data->crateHeader().swap(header); //use crate header created by a seb         
        std::get<2>(_tpc_seb_map[crate_number]).swap(crate_data);
        getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size*sizeof(raw_data_type));
        getGlobalHeader().setEventNumberCrate (crate_header.event_number);                
    }
    else if(crate_type == SystemDesignator::LASER_SYSTEM)
    {
      raw_fragment_data_t& tpm_fragment=fragment;      
      artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* tpm_fragment.begin());
      ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());

      //this is klugey, and inefficient, but it's the best I got right now --- WK
      ub_LaserData mydata;
      mydata.setData((char*)(&(*(tpm_fragment.begin()+artdaq_header->metadata_word_count+
				 artdaq_fragment_header::num_words()))));
      _laser_seb_map.emplace(crate_number,mydata);
    }

    getGlobalHeader().setNumberOfSEBs((uint8_t)(_tpc_seb_map.size() + _pmt_seb_map.size()));    
    updateDTHeader();

    } catch(datatypes_exception &e) {
	throw ;
    }catch(std::exception &ex) {
	   std::ostringstream os;
	   os << "Caught exception in  ub_EventRecord::addFragment(). Message:";
	   os << ex.what();
	   std::cerr << os.str() << std::endl;
	   
            throw datatypes_exception(os.str());
    } catch(...) {
    	   std::ostringstream os;
	   os << "Caught unknown exception in  ub_EventRecord::addFragment()";
	   std::cerr << os.str() << std::endl;

           throw datatypes_exception(os.str());
    }    
}

void ub_EventRecord::gps_sign_adjust(int& mu, int& nano)
{
  int mu_here = mu%1000000;  // always positive. Make no correction if it's 0.
  int nano_here = nano%1000; // this can be either sign in C++ (not so in python).
  if ( nano_here<0 && mu_here>0 )
    {
      mu_here-=1;
      nano_here+=1000;
    }

  // mod again and force to be positive just to be sure.
  mu = std::max(mu_here%1000000,0); 
  nano = std::max(nano_here%1000,0);
}

const ub_EventRecord::tpc_map_t ub_EventRecord::getTPCSEBMap() const noexcept
{
    tpc_map_t retMap;
    for(auto& tpc : _tpc_seb_map)
        retMap.emplace(tpc.first,*std::get<2>(tpc.second));
    return retMap;
}
const ub_EventRecord::pmt_map_t ub_EventRecord::getPMTSEBMap() const noexcept
{
    pmt_map_t retMap;
    for(auto& pmt : _pmt_seb_map)
        retMap.emplace(pmt.first,*std::get<2>(pmt.second));
    return retMap;
}
const ub_EventRecord::trig_map_t ub_EventRecord::getTRIGSEBMap() const noexcept
{
    trig_map_t retMap;
    for(auto& trg : _trigger_seb_map)
        retMap.emplace(trg.first,*std::get<2>(trg.second));
    return retMap;
}

ub_EventRecord::laser_map_t const& ub_EventRecord::getLASERSEBMap() const noexcept
{
  return _laser_seb_map;
}


void ub_EventRecord::getFragments(fragment_references_t& fragments) const noexcept
{
    uint16_t serialization_mask=_crate_serialization_mask.load();

    for(auto& tpc : _tpc_seb_map)
    {
        if(CHECK_BIT(serialization_mask,tpc.first))
            fragments.emplace_back(&std::get<0>(tpc.second));
    }
    
    for(auto& pmt : _pmt_seb_map)
    {
        if(CHECK_BIT(serialization_mask,pmt.first))
            fragments.emplace_back(&std::get<0>(pmt.second));
    }    
  
    for(auto& trg : _trigger_seb_map)
    {
        // if(CHECK_BIT(serialization_mask,trg.first))       // NJT: Always keep trigger data!
            fragments.emplace_back(&std::get<0>(trg.second));
    }

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
        fragments.begin(),fragments.end(),0u,[](size_t total, raw_fragment_data_t const* fragment) {
            return total+fragment->size()*sizeof(fragment_value_type_t);
        });

        _bookkeeping_header.event_global_header_word_offset=
            ub_event_header_wordcount+
            _bookkeeping_header.raw_event_fragments_wordcount+
            _bookkeeping_header.event_fragment_count*ub_size_t_wordcount;

        _bookkeeping_header.total_event_wordcount=_bookkeeping_header.event_global_header_word_offset
                //+ ADD GLOBAL HEADER SIZES
                +ub_event_trailer_wordcount;

        _bookkeeping_header.event_format_version= gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;

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

        if(_trigger_seb_map.size() !=event_record._trigger_seb_map.size())
            throw datatypes_exception( make_compare_message("_trigger_seb_map", "size", _trigger_seb_map.size(),event_record._trigger_seb_map.size()));

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
    auto lasers = getLASERSEBMap();

    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n TPC fragment count=" << tpcs.size();
    os << "\n PMT fragment count=" << pmts.size() << std::endl;
    os << "\n LASER fragment count=" << lasers.size() << std::endl;
    os << _global_header.debugInfo() << std::endl;

    os << "\nTrigger Counter";
    os << _trigger_counter.debugInfo();
    os << "\nTRG fragments";
    for(auto const& trg : _trigger_seb_map){
        raw_fragment_data_t const& tpm_fragment=std::get<0>(trg.second);
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        os << "\n" <<  crate_header_t::getHeaderFromFragment(data).debugInfo();
        os << "\n" <<  std::get<2>(trg.second)->debugInfo();
    }

    os << "\tSWTrigger Outputs";
    for (auto const& o : _swtrigger_output_vector)
      os << o.debugInfo();

    os << _beam_record.debugInfo() << std::endl;

    os << "\nTPC fragments";
    for(auto const& tpc : _tpc_seb_map){
        raw_fragment_data_t const& tpm_fragment=std::get<0>(tpc.second);
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        os << "\n" <<  crate_header_t::getHeaderFromFragment(data).debugInfo();
        os << "\n" <<  std::get<2>(tpc.second)->debugInfo();
    }

    os << "\nPMT fragments";
    for(auto const& pmt : _pmt_seb_map){
        raw_fragment_data_t const& tpm_fragment=std::get<0>(pmt.second);
        ub_RawData data(tpm_fragment.begin(),tpm_fragment.end());
        os << "\n" <<  crate_header_t::getHeaderFromFragment(data).debugInfo();
        os << "\n" <<  std::get<2>(pmt.second)->debugInfo();
    }

    os << "\nLASER fragments";
    for(auto const& laser : _laser_seb_map){
      os << "\n Laser crate number is " << laser.first;
      os << "\n" << (laser.second).debugInfo();
    }

    return os.str();
}
