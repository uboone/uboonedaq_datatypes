#include "ub_Trigger_ChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


template<>
bool ub_MarkedRawChannelData<ub_Trigger_Data_v6,empty_trailer>::isValid() noexcept
{
    return true;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

ub_Trigger_ChannelData_v6::ub_Trigger_ChannelData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}
