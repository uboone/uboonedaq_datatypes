#include "ub_CardData.h"
#include "ub_ChannelDataCreatorHelperClass.h"

using namespace gov::fnal::uboone::datatypes;

void ub_CardData::CreateMarkedRawCardData(){
  if(!_markedRawCardData)
    _markedRawCardData.swap(ub_MarkedRawCardData::CreateMarkedRawCardData(_version,_rawCardData));
}

std::vector<ub_RawDataWord_t> const& ub_CardData::getDataVector(){
  CreateMarkedRawCardData();
  ub_RawData_t const& data_iters = _markedRawCardData->data();
  std::vector<ub_RawDataWord_t> const& dataVector(data_iters.begin(),data_iters.end());
  return dataVector;
}

void ub_CardData::FillCardDataVector(){
  if(_channelDataVector.size()>0) return;

  CreateMarkedRawCardData();
  ub_ChannelDataCreatorHelperClass cdchc(_version,_markedRawCardData->data());
  _channelDataVector = cdchc.getChannelDataVector();
}

