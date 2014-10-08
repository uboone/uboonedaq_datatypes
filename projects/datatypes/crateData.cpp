#include "crateData.h"
#include "cardDataCreatorHelperClass.h"

using namespace gov::fnal::uboone::datatypes;

crateData::crateData(  ub_VersionWord_t const& version, 
		       ub_RawData_t const& rd ):
  _version(version), _rawCrateData(rd){}


void crateData::CreateMarkedRawCrateData(){
  if(!_markedRawCrateData)
    _markedRawCrateData.swap(ub_MarkedRawCrateData::CreateMarkedRawCrateData(_version,_rawCrateData));
}


uint32_t const& crateData::getHeaderWord(){
  CreateMarkedRawCrateData();
  return _markedRawCrateData->getHeaderWord();
}

uint32_t const& crateData::getTrailerWord(){
  CreateMarkedRawCrateData();
  return _markedRawCrateData->getTrailerWord();
}

std::vector<ub_RawDataWord_t> const& crateData::getDataVector(){
  CreateMarkedRawCrateData();
  ub_RawData_t const& data_iters = _markedRawCrateData->data();
  std::vector<ub_RawDataWord_t> const& dataVector(data_iters.begin(),data_iters.end());
  return dataVector;
}

void crateData::FillCardDataVector(){
  if(_cardDataVector.size()>0) return;

  CreateMarkedRawCrateData();
  CardDataCreatorHelperClass cdchc(_version,_markedRawCrateData->data());
  _cardDataVector = cdchc.getCardDataVector();
}
