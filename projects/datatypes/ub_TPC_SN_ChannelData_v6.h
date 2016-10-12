#ifndef _UBOONE_TYPES_TPC_SN_CHANELDATA_V6_H
#define _UBOONE_TYPES_TPC_SN_CHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_TPC_SN_ChannelHeaderTrailer_v0.h"
#include "ub_TPC_SN_PacketData_v6.h"
#include "empty_trailer.h"
#include <bitset>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_SN_ChannelData_v6 final: public ub_MarkedRawChannelData<ub_TPC_SN_ChannelHeader,empty_trailer>
{
public:
    ub_TPC_SN_ChannelData_v6(ub_RawData const& rawdata);
    ub_TPC_SN_ChannelData_v6 ( ub_TPC_SN_ChannelData_v6 && ) = default;


    void dissectPackets() throw(datatypes_exception);    
    std::vector<ub_TPC_SN_PacketData_v6> packets_;

    /// Huffman decompression. Available as T= uint_16, int16, uint_32, int32, float, and double. 
    /// This creates a single waveform for the whole 3200-word window. Wasteful, but compatable with
    /// The regular TPC calls.
    template<typename T>
     void decompress(std::vector<T>& uncompressed) const throw(datatypes_exception);  

};


template<typename T>
void ub_TPC_SN_ChannelData_v6::decompress(std::vector<T>& uncompressed) const throw(datatypes_exception)
{
  const size_t kMaxBufferSize = 3200;
  
  uncompressed.resize(0); // Set size.
  uncompressed.reserve(kMaxBufferSize);  // Enlarge if buffer is too small. No effect if buffer is already large.
  /// Loop through packets and decompress each.
  for(auto it = packets_.begin(); it!=packets_.end(); it++ ) {
    it->decompress_into(uncompressed);
  }

} 

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
