#ifndef _UBOONE_TYPES_PMTWINDOWDATA_V6_H
#define _UBOONE_TYPES_PMTWINDOWDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_TPC_ChannelHeaderTrailer_v0.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


class ub_PMT_WindowData_v6 final: public ub_MarkedRawChannelData<ub_TPC_ChannelHeader,ub_TPC_ChannelTrailer>
{
public:
    ub_PMT_WindowData_v6(ub_RawData const& rawdata); 
    ub_PMT_WindowData_v6 ( ub_PMT_WindowData_v6 && ) = default;

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_PMTWINDOWDATA_V6_H
