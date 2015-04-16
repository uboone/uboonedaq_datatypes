#ifndef _UBOONE_TYPES_MARKEDRAWCHANELDATA_H
#define _UBOONE_TYPES_MARKEDRAWCHANELDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelHeaderTrailer_v0.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_MarkedRawChannelData : public ub_MarkedRawDataBlock<ub_ChannelHeader,ub_ChannelTrailer>
{
public:
    explicit ub_MarkedRawChannelData(ub_RawData const rawdata);

    uint16_t getChannelHeaderWord() const noexcept;
    uint16_t getChannelTrailerWord() const noexcept;
    uint16_t getChannelNumber() const noexcept;

    ub_MarkedRawChannelData() = delete;
    ub_MarkedRawChannelData& operator=(ub_MarkedRawChannelData const &) = delete;

    std::string debugInfo()const noexcept;

private:
    bool isValid() noexcept;
    bool canFullyDissect() noexcept;
private:
    bool _isValid;
    bool _isFullyDissected;
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_H
