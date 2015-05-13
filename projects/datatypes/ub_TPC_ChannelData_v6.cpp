#include "ub_TPC_ChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

ub_TPC_ChannelData_v6::ub_TPC_ChannelData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}




template void ub_TPC_ChannelData_v6::decompress<uint16_t>(std::vector<uint16_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< int16_t>(std::vector< int16_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress<uint32_t>(std::vector<uint32_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< int32_t>(std::vector< int32_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< float  >(std::vector< float  >&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< double >(std::vector< double >&) const throw(datatypes_exception);

