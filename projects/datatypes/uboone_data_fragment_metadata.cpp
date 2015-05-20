#include "uboone_data_fragment_metadata.h"
#include "raw_data_access.h"

using namespace gov::fnal::uboone::datatypes;


extern "C"  DissectorFactory& getDissectorFactory()
{
     static DissectorFactory theDissectorFactory;
     return theDissectorFactory;
}


typedef  DissectorAdapter<ub_PMT_CrateData_v6> ub_PMT_CrateData_v6_dissector_t;
typedef  DissectorAdapter<ub_TPC_CrateData_v6> ub_TPC_CrateData_v6_dissector_t;
typedef  DissectorAdapter<ub_Trigger_CrateData_v6> ub_Trigger_CrateData_v6_dissector_t;

DissectorFactory::DissectorFactory()
{
  registerDissector("tpc",6,&ub_TPC_CrateData_v6_dissector_t::buildCrateHeader);
  registerDissector("pmt",6,&ub_PMT_CrateData_v6_dissector_t::buildCrateHeader);
  registerDissector("trigger",6,&ub_Trigger_CrateData_v6_dissector_t::buildCrateHeader);
}

DissectorFactory::~DissectorFactory() { 
_dissectors.clear(); 
}

void DissectorFactory::registerDissector(std::string const& name, uint8_t const& version, crate_header_builder builder)
{
     auto result = _dissectors.find(name);
     
      if(result != _dissectors.end()) {
	if(result->second.first > version) //we already have one
	  return;
      }
      
    _dissectors.emplace(name,std::make_pair(version,builder));
}

crate_header_t DissectorFactory::buildCrateHeaderFromRawData(std::string const& name, uint8_t const& version, ub_RawData const& rawdata, bool initializeHeaderFromRawData)
{
    UNUSED(version);
    auto result = _dissectors.find(name);
    assert( result != _dissectors.end() ); 
    assert(result->second.first<=version); 
    return result->second.second(rawdata,initializeHeaderFromRawData);
}

   

/*

void rawFragmentDMASource::processEventFragment(EventFragmentRPtr& fragment)
{
  gov::fnal::uboone::datatypes::getDataFactory().
  
   ub_RawData raw_data(fragment->dataBegin(),fragment->dataEnd());
   std::size_t initialWordCount=raw_data.size();
      
   ub_CrateData_t crate(raw_data);
   std::size_t validatedlWordCount=crate.getSizeOfDissectableCrateData();
   
   assert(initialWordCount>=validatedlWordCount);
   
   fragment->resize(validatedlWordCount);
   
   auto header = fragment->metadata<ub_CrateData_t::ub_CrateHeader_t>();
   
   struct timeval t_end;        
   //update crate header
   header->card_count=crate.getCards().size();
   header->complete=1;
   gettimeofday(&t_end,NULL);
   header->seb_time_sec=t_end.tv_sec;
   header->event_number=crate.crateHeader()->event_number;
   header->frame_number=crate.crateHeader()->frame_number;
   header->seb_time_usec=t_end.tv_usec;
   header->size=fragment->dataSizeBytes();   
   header->crate_number=std::atoi(_machinename.erase(0,_machinename.length()-2).c_str());
   header->crateBits|= ( header->crate_number&0x000f);

//#define _USE_FRAME_NUMBER_AS_SEQUENCE_ID 1   
#ifndef _USE_FRAME_NUMBER_AS_SEQUENCE_ID    
   fragment->setSequenceID( getNextFragmentSequenceId() ); 
#else
   fragment->setSequenceID( crate.crateHeader()->frame_number );
#endif
   updateEventFragmentHeader_Derived(fragment);
   
   fragment->updateDTHeader<ub_CrateData_t>();
   
#if 1
   {  //debug printout of the fragment header
      ub_RawData raw_data(fragment->headerBegin(),fragment->dataEnd());
      ub_CrateData_t::ub_CrateHeader_t const& valid_header=ub_CrateData_t::ub_CrateHeader_t::getHeaderFromFragment(raw_data);
      mf::LogDebug ( "rawFragmentDMASource" ) <<  valid_header.debugInfo();
   }
#endif   
   
}
*/
