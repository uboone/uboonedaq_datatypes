#ifndef _UBOONE_TYPES_PMTCHANELDATA_V6_H
#define _UBOONE_TYPES_PMTCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


class ub_PMT_ChannelData_v6 final: public ub_MarkedRawChannelData
{
public:
    ub_PMT_ChannelData_v6(ub_RawData const rawdata);
    
    ub_PMT_ChannelData_v6() = delete;
    ub_PMT_ChannelData_v6& operator=(ub_PMT_ChannelData_v6 const &) = delete;
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
