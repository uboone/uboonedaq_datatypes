#include "ub_MarkedRawCrateData_v6.h"

using namespace gov::fnal::uboone::datatypes;

ub_MarkedRawCrateData_v6::ub_MarkedRawCrateData_v6(ub_RawData_t const& rd){

  size_t iter_fwd = sizeof(ub_CrateHeader_t_v6)/rd.typeSize();
  ub_CrateHeader_t_v6 const& my_header = *(reinterpret_cast<ub_CrateHeader_t_v6 const*>(rd.begin()));
  assert(my_header.header == 0xffffffff);
  
  size_t iter_rwd = sizeof(ub_CrateTrailer_t_v6)/rd.typeSize();
  ub_CrateTrailer_t_v6 const& my_trailer = *(reinterpret_cast<ub_CrateTrailer_t_v6 const*>(rd.end()-iter_rwd));
  assert(my_trailer.trailer == 0xe0000000);
  
  _marked_raw_data_block = 
    ub_MarkedDataBlock<ub_CrateHeader_t_v6,ub_CrateTrailer_t_v6>( rd,
								  ub_RawData_t(rd.begin()+iter_fwd,rd.end()-iter_rwd),
								  rd.begin(),
								  rd.end()-iter_rwd );
}
