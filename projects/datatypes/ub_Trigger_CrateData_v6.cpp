#include "ub_Trigger_CrateData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <>
bool  ub_MarkedRawCrateData<ub_Trigger_CardData_v6,empty_header,ub_Trigger_Trailer_v6>::isValid() noexcept
{
    return true;
}

bool ub_Trigger_CrateData_v6::compare(ub_Trigger_CrateData_v6 const & crate_data, bool do_rethrow) const
{
    try
    {
      return ub_MarkedRawCrateData<ub_Trigger_CardData_v6,empty_header,ub_Trigger_Trailer_v6>::compare(crate_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_Trigger_CrateData_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_Trigger_CrateData_v6::compare()");
        else
            return false;
    }
    return true;
}

ub_Trigger_CrateData_v6::ub_Trigger_CrateData_v6(ub_RawData const& rawdata, bool initializeHeaderFromRawData):
  ub_MarkedRawCrateData<ub_Trigger_CardData_v6,empty_header,ub_Trigger_Trailer_v6>(rawdata, initializeHeaderFromRawData)
{
}

ub_Trigger_CardData_v6 const& ub_Trigger_CrateData_v6::getTriggerCardData() const
{
  if( getCards().size() != 1)
    throw datatypes_exception("Exception in ub_Trigger_CrateData_v6::getTriggerCardData(): getCards() data size not 1.");

  return getCards().at(0);
}

ub_Trigger_Header_v6 const& ub_Trigger_CrateData_v6::getTriggerHeader() const
{
  return getTriggerCardData().header();
}

ub_Trigger_Data_v6 const& ub_Trigger_CrateData_v6::getTriggerData() const
{
  if( getCards().size() != 1)
    throw datatypes_exception("Exception in ub_Trigger_CrateData_v6::getTriggerCardData(): getCards() data size not 1.");

  return getCards().at(0).getTriggerData();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

