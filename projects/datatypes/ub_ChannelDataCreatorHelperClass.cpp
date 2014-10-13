#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_ChannelData.h"
#include "ub_MarkedRawChannelData.h"

using namespace gov::fnal::uboone::datatypes;

ub_ChannelDataCreatorHelperClass::ub_ChannelDataCreatorHelperClass(ub_VersionWord_t const version, 
								   ub_RawData_t const rd){


  ub_RawData_t new_rawdata(rd.begin(),rd.begin());
  
  while(new_rawdata.end()!=rd.end()){
    
    new_rawdata = ub_RawData_t(new_rawdata.begin(),rd.end());
    _markedRawChannelData.swap(ub_MarkedRawChannelData::CreateMarkedRawChannelData(version,new_rawdata));
    
    new_rawdata = _markedRawCardData->rawdata();
    _channelDataVector.emplace_back(version,new_rawdata);

    if( std::distance(new_data.end(),rd.end()) < 
	(sizeof(_markedRawCardData->header())+sizeof(_markedRawCardData->trailer()))/sizeof(ub_RawDataWord_t)) 
      break;
  }

}
