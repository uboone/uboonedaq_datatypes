#include "ub_MarkedRawChannelData.h"

using namespace gov::fnal::uboone::datatypes;

bool ub_MarkedRawChannelData::isValid() noexcept
{
    if( header().header_mark!=0x40 )
        return false;
    if( trailer().trailer_mark!=0x50 )
        return false;
    return true;
}

std::string ub_MarkedRawChannelData::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << header().debugInfo();
    os << trailer().debugInfo();
    
     if(rawdata().size() < 0x200)
         os << ub_MarkedRawDataBlock::debugInfo();
     else
         os << debugInfoShort(rawdata());
    
    return os.str();
}

ub_MarkedRawChannelData::ub_MarkedRawChannelData(ub_RawData const rawdata):
    ub_MarkedRawDataBlock<ub_ChannelHeader,ub_ChannelTrailer>(rawdata),
    _isValid {isValid()},_isFullyDissected {canFullyDissect()}
{
}

uint16_t ub_MarkedRawChannelData::getChannelHeaderWord() const noexcept {
    return header().channel_mark;
}

uint16_t ub_MarkedRawChannelData::getChannelTrailerWord() const noexcept {
    return trailer().channel_mark;
}

uint16_t ub_MarkedRawChannelData::getChannelNumber() const noexcept {
    return header().getChannelNumber();
}

bool ub_MarkedRawChannelData::canFullyDissect() noexcept {
    return true;
}
