#include "ub_PMT_WindowHeader_v6.h"

using namespace gov::fnal::uboone::datatypes;

std::string ub_PMT_WindowHeader_v6::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << std::hex << std::setfill('0') << std::setw(4);
    os << "  Channel[" << channel_number<< "], Discriminator["  << discriminator
       << ", " << getDiscriminatorType() <<"], Marker[" << marker1;
    os << "], RAW[0x"<< ch_and_disc << "]" << std::endl;
    os << "  Frame[" << frame << "], Sample["  << getSample()
       << "], Markers[" << marker2 << ", " << marker3;
    os << "], RAW[0x"<< frame_and_sample1 << ", 0x" << sample2 << "]" << std::endl;

    return os.str();
}
