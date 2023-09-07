#include "ub_Trigger_CardData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<>
bool ub_MarkedRawCardData<ub_Trigger_ChannelData_v6,ub_Trigger_Header_v6,empty_trailer>::isValid() noexcept
{
    return true;
}
// template<>
// bool ub_MarkedRawCardData<ub_Trigger_ChannelData_v6,ub_Trigger_Header_v6,empty_trailer>::_do_dissect=true;


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

bool ub_Trigger_CardData_v6::compare(ub_Trigger_CardData_v6 const& card_data,bool do_rethrow ) const
{
    try {
        return ub_MarkedRawCardData<ub_Trigger_ChannelData_v6,ub_Trigger_Header_v6,empty_trailer>::compare(card_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_Trigger_CardData_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_Trigger_CardData_v6::compare()");
        else
            return false;
    }
    return true;
}


ub_Trigger_CardData_v6::ub_Trigger_CardData_v6(ub_RawData const& rawdata):
    ub_MarkedRawCardData<ub_Trigger_ChannelData_v6,ub_Trigger_Header_v6,empty_trailer>(rawdata)
{
}

bool ub_Trigger_CardData_v6::operator==(ub_Trigger_CardData_v6 const& card_data) const {
    return compare(card_data,false);
}

uint32_t ub_Trigger_CardData_v6::getTrigNumber() const noexcept {
    return header().getTriggerNumber();
}
uint32_t ub_Trigger_CardData_v6::getFrame() const noexcept {
    return header().getFrame();
}
uint16_t ub_Trigger_CardData_v6::getSample() const noexcept {
    return header().getSampleNumber();
}
uint16_t ub_Trigger_CardData_v6::getRemainder() const noexcept {
    return header().get16MHzRemainderNumber();
}

ub_Trigger_Data_v6 const& ub_Trigger_CardData_v6::getTriggerData() const
{
  if( getChannels().size() != 1)
    throw datatypes_exception("Exception in ub_Trigger_CardData_v6::getTriggerData(): getChannels() data size not 1.");

  return getChannels().at(0).header();
}
