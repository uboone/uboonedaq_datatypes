#include "ub_CardDataCreatorHelperClass.h"
#include "ub_MarkedRawCardData.h"
#include "ub_CardData.h"

using namespace gov::fnal::uboone::datatypes;

ub_CardDataCreatorHelperClass::ub_CardDataCreatorHelperClass(ub_VersionWord_t const version, 
							     ub_RawData_t const rd){

  
  ub_RawData_t new_rawdata(rd.begin(),rd.begin());
  
  while(new_rawdata.end()!=rd.end()){
    
    new_rawdata = ub_RawData_t(new_rawdata.begin(),rd.end());
    _markedRawCardData.swap(ub_MarkedRawCardData::CreateMarkedRawCardData(version,new_rawdata));
    uint32_t wordCount = _markedRawCardData->getWordCount();
    new_rawdata = ub_RawData_t(_markedRawCardData->rawdata().begin(),
			       _markedRawCardData->data().begin()+wordCount+sizeof(_markedRawCardData->trailer())/sizeof(ub_RawDataWord_t));
    _cardDataVector.emplace_back(version,new_rawdata);

    if( std::distance(new_data.end(),rd.end()) < sizeof(_markedRawCardData->header())/sizeof(ub_RawDataWord_t)) break;
  }

}
