#ifndef _UBOONE_TYPES_CHANNEL_H
#define _UBOONE_TYPES_CHANNEL_H 1

#include "uboonetypes_Base.h"
#include "ub_MarkedRawChannelData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

/** CHANNEL Control Words.
    First 4 bits are 0100 (0101) for first (last) word of channel.
    Remaining 12 bits are the channel #.
    Use this for degugging on seb side.
**/
typedef struct ub_ChannelHeader_v6
{
    uint16_t channel_begin; /* OR'd channel|"first"*/
} ub_ChannelHeader_t_v6;

typedef struct ub_ChannelTrailer_v6
{
    uint16_t channel_end; /* OR'd channel|"first"*/
} ub_ChannelTrailer_t_v6;


class ub_MarkedRawChannelData_v6 : public ub_MarkedRawChannelData{

 public:
  ub_MarkedRawChannelData_v6(ub_RawData_t const& rawdata);

  ub_RawData_t const& data() const { return _marked_raw_data_block.data(); }
  ub_RawData_t const& rawdata(); const { return _marked_raw_data_block.rawdata(); }

  uint16_t const& getChannelHeaderWord() { return _marked_raw_data_block.header().channel_begin; }
  uint16_t const& getChannelTrailerWord() { return _marked_raw_data_block.trailer().channel_end; }

  uint16_t getChannelNumber() const { return _marked_raw_data_block.header().channel_begin & 0xfff; }

 private:
  ub_MarkedRawDataBlock<ub_ChannelHeader_t_v6,ub_ChannelTrailer_t_v6> _marked_raw_data_block;
};



}
}
}
}

#endif
