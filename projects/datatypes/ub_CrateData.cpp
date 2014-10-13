#include "ub_CrateData.h"
#include "ub_CardDataCreatorHelperClass.h"

using namespace gov::fnal::uboone::datatypes;

void ub_CrateData::CreateMarkedRawCrateData(){
  if(!_markedRawCrateData)
    _markedRawCrateData.swap(ub_MarkedRawCrateData::CreateMarkedRawCrateData(_version,_rawCrateData));
}

std::vector<ub_RawDataWord_t> const& ub_CrateData::getDataVector(){
  CreateMarkedRawCrateData();
  ub_RawData_t const& data_iters = _markedRawCrateData->data();
  std::vector<ub_RawDataWord_t> const& dataVector(data_iters.begin(),data_iters.end());
  return dataVector;
}

void ub_CrateData::FillCardDataVector(){
  if(_cardDataVector.size()>0) return;

  CreateMarkedRawCrateData();
  ub_CardDataCreatorHelperClass cdchc(_version,_markedRawCrateData->data());
  _cardDataVector = cdchc.getCardDataVector();
}
