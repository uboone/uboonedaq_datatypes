#include "ub_MarkedRawCrateData.h"

using namespace gov::fnal::uboone::datatypes;

std::uniqe_ptr<ub_MarkedRawCrateData> ub_MarkedRawCrateData::CreateMarkedRawCrateData(ub_VersionWord_t const& version,
										      ub_RawData_t const& rd){

  if(version==6)
    return std::unique_ptr<ub_MarkedRawCrateData_v6>(new ub_MarkedRawCrateData_v6(rd) );


  throw std::runtime_error("Invalid Raw Data Version Number in Crate Data");

}

ub_MarkedRawCrateData_v6::ub_MarkedRawCrateData_v6(ub_RawData_t const& rd){

  size_t iter_fwd = sizeof(xmit_header_t_v6)/rd.typeSize();
  xmit_header_t_v6 const& my_header = *(reinterpret_cast<xmit_header_t_v6 const*>(rd.begin()));
  assert(my_header.header == 0xffffffff);
  
  size_t iter_rwd = sizeof(xmit_trailer_t_v6)/rd.typeSize();
  xmit_trailer_t_v6 const& my_trailer = *(reinterpret_cast<xmit_trailer_t_v6 const*>(rd.end()-iter_rwd));
  assert(my_trailer.trailer == 0xe0000000);
  
  _marked_raw_data_block = 
    ub_MarkedDataBlock<xmit_header_t_v6,xmit_trailer_t_v6>( rd,
							    ub_RawData_t(rd.begin()+iter_fwd,rd.end()-iter_rwd),
							    rd.begin(),
							    rd.end()-iter_rwd );
}
