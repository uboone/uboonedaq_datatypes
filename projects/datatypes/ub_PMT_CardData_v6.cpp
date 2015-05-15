#include "ub_PMT_CardData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<>
bool ub_MarkedRawCardData<ub_PMT_WindowData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::isValid() noexcept
{
    return true;
}
template<>
bool ub_MarkedRawCardData<ub_PMT_WindowData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::_dissectChannels=true;


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

uint32_t ub_PMT_CardData_v6::getTrigFrame() const noexcept
{
    // Attempt to resolve the rollover situation: the lower 4 bits are given by the trigger frame, which should crudely match the upper bits from the course frame.
    // Here is how I resolved it --- Nathaniel
    uint32_t frameCourse = getFrame();
    uint32_t option1 = (getFrame()&0xFFFFFFF0) | (getTrigFrameMod16());
    int32_t diff = option1-frameCourse;
    if(diff > 8) return option1 - 0x10; // Solution is too high; rollover down
    if(diff < 8) return option1 + 0x10; // Solution is too low; rollover up.
    return option1; // if within 8 ticks, this solution is fine.
}

bool ub_PMT_CardData_v6::compare(ub_PMT_CardData_v6 const& card_data,bool do_rethrow ) const throw(datatypes_exception)
{
    try {
        return ub_MarkedRawCardData<ub_PMT_WindowData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::compare(card_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_PMT_CardData_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_PMT_CardData_v6::compare()");
        else
            return false;
    }
    return true;
}

ub_PMT_CardData_v6::ub_PMT_CardData_v6(ub_RawData const& rawdata):
    ub_MarkedRawCardData<ub_PMT_WindowData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>(rawdata) {}

bool ub_PMT_CardData_v6::operator==(ub_PMT_CardData_v6 const& card_data) const{
    return compare(card_data,false);
}

uint32_t ub_PMT_CardData_v6::getCardTrigFrameAndSampleWord() const noexcept {
    return header().trig_frame_and_sample;
}

uint32_t ub_PMT_CardData_v6::getID() const noexcept {
    return header().getID();
}
uint32_t ub_PMT_CardData_v6::getModule() const noexcept {
    return header().getModule();
}
uint32_t ub_PMT_CardData_v6::getEvent() const noexcept {
    return header().getEvent();
}
uint32_t ub_PMT_CardData_v6::getFrame() const noexcept {
    return header().getFrame();
}
uint32_t ub_PMT_CardData_v6::getChecksum() const noexcept {
    return header().getChecksum();
}
uint32_t ub_PMT_CardData_v6::getWordCount() const noexcept {
    return header().getWordCount();
}
uint32_t ub_PMT_CardData_v6::getTrigFrameMod16() const noexcept {
    return header().getTrigFrameMod16();
}
uint32_t ub_PMT_CardData_v6::getTrigSample() const noexcept {
    return header().getTrigSample();
}
uint16_t ub_PMT_CardData_v6::getDataStartMarker() const noexcept {
  return header().getDataStartMarker();
}
uint16_t ub_PMT_CardData_v6::getDataEndMarker() const noexcept {
  return trailer().getDataEndMarker();
}
