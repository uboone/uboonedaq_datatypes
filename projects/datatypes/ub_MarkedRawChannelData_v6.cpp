#include "ub_MarkedRawChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

ub_MarkedRawChannelData_v6::ub_MarkedRawChannelData_v6(ub_RawData_t const& rd){

  size_t iter_fwd = sizeof(ub_ChannelHeader_t_v6)/rd.typeSize();
  ub_ChannelHeader_t_v6 const& my_header = *(reinterpret_cast<ub_ChannelHeader_t_v6 const*>(rd.begin()));
  assert(my_header.header&0xf000==0x4000);

  const uint16_t ch = my_header.header&0xfff;

  size_t iter_rwd = sizeof(ub_ChannelTrailer_t_v6)/rd.typeSize();
  ub_ChannelTrailer_t_v6 const& my_trailer = *(reinterpret_cast<ub_ChannelTrailer_t_v6 const*>(rd.end()-iter_rwd));

  //if the right trailer is at the end of the data block, we can skip walking through the data
  if(my_trailer.trailer&0xf000==0x5000 && my_trailer.trailer&0xfff==ch){
    _marked_raw_data_block = 
      ub_MarkedDataBlock<ub_ChannelHeader_t_v6,ub_ChannelTrailer_t_v6>( rd,
									ub_RawData_t(rd.begin()+iter_fwd,rd.end()-iter_rwd),
									rd.begin(),
									rd.end()-iter_rwd);
    return;
  }
  
  //else, we have to walk through the data
  for(ub_RawData_t::const_iterator iter = rd.begin()+1; iter!=rd.end(); iter++){
    ub_ChannelHeader_t_v6 const& maybe_trailer = *(reinterpret_cast<ub_ChannelHeader_t_v6 const*>(iter));

    if(maybe_trailer.trailer&0xf000==0x5000) {
      assert(maybe_trailer.trailer&0xfff==ch);
      _marked_raw_data_block = 
	ub_MarkedDataBlock<ub_ChannelHeader_t_v6,ub_ChannelTrailer_t_v6>( ub_RawData_t(rd.begin(),iter+1),
									  ub_RawData_t(rd.begin()+iter_fwd,iter),
									  rd.begin(),
									  iter );
      return;
    }

  }

  throw std::runtime_error("Invalid Raw Channel Data: header word found, but no trailer word.");
}
