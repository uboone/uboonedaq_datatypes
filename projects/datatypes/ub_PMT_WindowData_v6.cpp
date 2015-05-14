#include "ub_PMT_WindowData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


template<>
bool ub_MarkedRawChannelData<ub_PMT_WindowHeader_v6,empty_trailer>::isValid() noexcept
{
    if( header().marker1!=1 )
        return false;
    if( header().marker2!=2 )
        return false;
    if( header().marker3!=2 )
        return false;
    if( header().empty1!=0 )
        return false;
    if( header().empty2!=0 )
        return false;
    return true;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

ub_PMT_WindowData_v6::ub_PMT_WindowData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}
