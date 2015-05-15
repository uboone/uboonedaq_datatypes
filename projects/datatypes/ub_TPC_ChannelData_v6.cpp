#include "ub_TPC_ChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


template<>
bool ub_MarkedRawChannelData<ub_TPC_ChannelHeader,ub_TPC_ChannelTrailer>::isValid() noexcept
{
    if( header().header_mark!=0x40 )
        return false;
    if( trailer().trailer_mark!=0x50 )
        return false;
    return true;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

ub_TPC_ChannelData_v6::ub_TPC_ChannelData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}


uint16_t ub_TPC_ChannelData_v6::getChannelHeaderWord() const noexcept {
    return header().channel_mark;
}

uint16_t ub_TPC_ChannelData_v6::getChannelTrailerWord() const noexcept {
    return trailer().channel_mark;
}



template void ub_TPC_ChannelData_v6::decompress<uint16_t>(std::vector<uint16_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< int16_t>(std::vector< int16_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress<uint32_t>(std::vector<uint32_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< int32_t>(std::vector< int32_t>&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< float  >(std::vector< float  >&) const throw(datatypes_exception);
template void ub_TPC_ChannelData_v6::decompress< double >(std::vector< double >&) const throw(datatypes_exception);

