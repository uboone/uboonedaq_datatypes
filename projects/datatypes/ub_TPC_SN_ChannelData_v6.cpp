#include "ub_TPC_SN_ChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

  template<>
  bool ub_MarkedRawChannelData<ub_TPC_SN_ChannelHeader,empty_trailer>::isValid() noexcept
  {
      if( header().header_mark!=0x40 )
          return false;

      return true;
  }

  void ub_TPC_SN_ChannelData_v6::dissectPackets() throw(datatypes_exception)
  {
    packets_.clear();
    ub_RawData curr_rawData {data().begin(),data().end()};
    uint16_t header_word = *curr_rawData.begin();
      // Look at first word, does it match a window marker?
      // 0x4??? Beginning of channel (wire) data. It contains the FEM channel number in the 5:0 bits and the frame number in the 11:6 bits.
      // Note there is not “end of channel data” word unlike the NU stream. Here, the end of channel data is marked by the appearance of the
      // next beginning of channel data word,

    if((header_word & 0xf000)!=0x1000) {
       std::stringstream ss; ss << "Invalid SN packet header: " <<hex(4,header_word) << " (expected 0x1xxx)";
       throw datatypes_exception(ss.str());
    }
          
    while(curr_rawData.size()>0) {
      // look for next header to end the block:
      ub_RawData::const_iterator curr_position=curr_rawData.begin()+1;
      while( (curr_position != curr_rawData.end()) && (((*curr_position)&0xf000)!=0x1000) ){ curr_position++; };
      ub_RawData data {curr_rawData.begin(),curr_position};                            
      packets_.push_back(data);
      curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
      
    }
  }
    
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

ub_TPC_SN_ChannelData_v6::ub_TPC_SN_ChannelData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}


// uint16_t ub_TPC_SN_ChannelData_v6::getChannelHeaderWord() const noexcept {
//     return header().channel_mark;
// }




template void ub_TPC_SN_ChannelData_v6::decompress<uint16_t>(std::vector<uint16_t>&) const throw(datatypes_exception);
template void ub_TPC_SN_ChannelData_v6::decompress< int16_t>(std::vector< int16_t>&) const throw(datatypes_exception);
template void ub_TPC_SN_ChannelData_v6::decompress<uint32_t>(std::vector<uint32_t>&) const throw(datatypes_exception);
template void ub_TPC_SN_ChannelData_v6::decompress< int32_t>(std::vector< int32_t>&) const throw(datatypes_exception);
template void ub_TPC_SN_ChannelData_v6::decompress< float  >(std::vector< float  >&) const throw(datatypes_exception);
template void ub_TPC_SN_ChannelData_v6::decompress< double >(std::vector< double >&) const throw(datatypes_exception);

