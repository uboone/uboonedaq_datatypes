#ifndef _UBOONE_TYPES_TPCCHANELDATA_V6_H
#define _UBOONE_TYPES_TPCCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_ChannelData_v6 final: public ub_MarkedRawChannelData
{
public:
    ub_TPC_ChannelData_v6(ub_RawData const& rawdata);
    ub_TPC_ChannelData_v6 ( ub_TPC_ChannelData_v6 && ) = default;
    
    template<typename T>
     std::shared_ptr< std::vector<T> > decompress() const throw(datatypes_exception); /// Huffman decompression. Available as T= uint_16, int16, uint_32, int32, float, and double. 
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
