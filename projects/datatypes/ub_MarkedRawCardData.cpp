#include "ub_MarkedRawCardData.h"

using namespace gov::fnal::uboone::datatypes;

std::uniqe_ptr<ub_MarkedRawCardData> ub_MarkedRawCrateData::CreateMarkedRawCardData(ub_VersionWord_t const& version,
										    ub_RawData_t const& rd){

  if(version==6)
    return std::unique_ptr<ub_MarkedRawCardData_v6>(new ub_MarkedRawCardData_v6(rd) );


  throw std::runtime_error("Invalid Raw Data Version Number in Card Data");

}
