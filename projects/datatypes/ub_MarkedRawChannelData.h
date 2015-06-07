#ifndef _UBOONE_TYPES_MARKEDRAWCHANELDATA_H
#define _UBOONE_TYPES_MARKEDRAWCHANELDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
//#include "ub_ChannelHeaderTrailer_v0.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename HEADER,typename TRAILER>
class ub_MarkedRawChannelData : public ub_MarkedRawDataBlock<HEADER,TRAILER>
{
public:
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::header_type channel_header_type;
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer_type channel_trailer_type;

    static constexpr size_t size_of_data_overhead() {
        return size_of<channel_header_type>() + size_of<channel_trailer_type>();
    }

    explicit ub_MarkedRawChannelData(ub_RawData const& rawdata):
    ub_MarkedRawDataBlock<HEADER,TRAILER>(rawdata),
      _isValid {isValid()},_isFullyDissected {canFullyDissect()}{}

    //uint16_t getChannelHeaderWord() const noexcept;
    //uint16_t getChannelTrailerWord() const noexcept;
    uint16_t getChannelNumber() const noexcept {
      return  ub_MarkedRawDataBlock<HEADER,TRAILER>::header().getChannelNumber();
    }

    ub_MarkedRawChannelData() = delete;

    ub_MarkedRawChannelData ( ub_MarkedRawChannelData const& ) = delete;
    ub_MarkedRawChannelData ( ub_MarkedRawChannelData && ) = default;
    ub_MarkedRawChannelData& operator= ( ub_MarkedRawChannelData const& ) = delete;
    ub_MarkedRawChannelData& operator= ( ub_MarkedRawChannelData  && ) = default;
    
    std::string debugInfo()const noexcept;
    bool        wasDissected() const { return _isFullyDissected; }

private:
    bool isValid() noexcept;
    bool canFullyDissect() noexcept;
private:
    bool _isValid;
    bool _isFullyDissected;
};

template <typename HEADER,typename TRAILER>
bool ub_MarkedRawChannelData<HEADER,TRAILER>::canFullyDissect() noexcept
{
    return true;
}

template <typename HEADER,typename TRAILER>
std::string ub_MarkedRawChannelData<HEADER,TRAILER>::debugInfo()const noexcept
{
    std::ostringstream os;

    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::header().debugInfo();
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer().debugInfo();


    if(ub_MarkedRawDataBlock<HEADER,TRAILER>::rawdata().size() < 0x200)
      os << ub_MarkedRawDataBlock<HEADER,TRAILER>::debugInfo();
    else
      os << debugInfoShort(ub_MarkedRawDataBlock<HEADER,TRAILER>::rawdata());

    return os.str();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_H
