#include "uboonetypes_Card.h"

using namespace gov::fnal::uboone::datatypes;

std::uniqe_ptr<ub_MarkedRawCardData> ub_MarkedRawCrateData::CreateMarkedRawCardData(ub_VersionWord_t const& version,
										    ub_RawData_t const& rd){

  if(version==6)
    return std::unique_ptr<ub_MarkedRawCardData_v6>(new ub_MarkedRawCardData_v6(rd) );


  throw std::runtime_error("Invalid Raw Data Version Number in Card Data");

}

ub_MarkedRawCardData_v6::ub_MarkedRawCardData_v6(ub_RawData_t const& rd){

  size_t iter_fwd = sizeof(card_header_t_v6)/rd.typeSize();
  size_t iter_rwd = sizeof(card_trailer_t_v6)/rd.typeSize();
  _marked_raw_data_block = 
    ub_MarkedDataBlock<card_header_t_v6,card_trailer_t_v6>( rd,
							    ub_RawData_t(rd.begin()+iter_fwd,rd.end()-iter_rwd),
							    rd.begin(),
							    rd.end()-iter_rwd );

}

uint32_t ub_MarkedRawCardData_v6::getID() const { }

uint32_t ub_MarkedRawCardData_v6::getModule() const { }

uint32_t ub_MarkedRawCardData_v6::getEvent() const { }

uint32_t ub_MarkedRawCardData_v6::getFrame() const { }

uint32_t ub_MarkedRawCardData_v6::getChecksum() const { }

uint32_t ub_MarkedRawCardData_v6::getWordCount() const { }

uint32_t ub_MarkedRawCardData_v6::getTrigFrame() const { }

uint32_t ub_MarkedRawCardData_v6::getTrigFrameMod16() const { }

uint32_t ub_MarkedRawCardData_v6::getTrigSample() const { }
