#ifndef _UBOONE_TYPES_TPCSNCHANNELHEADERTRAILER_H
#define _UBOONE_TYPES_TPCSNCHANNELHEADERTRAILER_H 1

#include "uboone_data_utils.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
/** CHANNEL Control Words.
    First 4 bits are 0100 (0101) for first (last) word of channel.
    Remaining 12 bits are the channel #.
    Use this for degugging on seb side.
**/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct ub_TPC_SN_ChannelHeader final
{
    union {
        struct {
            uint16_t channel_number :6;
            uint16_t frame_number  :6; 
            uint16_t header_mark    :4;
        } ;
        uint16_t channel_mark=0xDEAD;
    } ;
    uint16_t getChannelNumber() const noexcept{
        return channel_number;
    }
    uint16_t getFrameNumber_6bit() const noexcept{
        return frame_number;
    }
    uint16_t getHeaderMark() const noexcept{
        return header_mark;
    }
    std::string debugInfo()const noexcept;
} ;
static_assert (sizeof(ub_TPC_SN_ChannelHeader) == 2, "ub_TPC_SN_ChannelHeader structure size is not correct.");

#pragma GCC diagnostic pop

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CHANELHEADERTRAILER_H
