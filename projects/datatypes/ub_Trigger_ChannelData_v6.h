#ifndef _UBOONE_TYPES_TRIGGERCHANELDATA_V6_H
#define _UBOONE_TYPES_TRIGGERCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_Trigger_DataTypes_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_Trigger_ChannelData_v6 final: public ub_MarkedRawChannelData<ub_Trigger_Data_v6,empty_trailer>
{
public:
    ub_Trigger_ChannelData_v6(ub_RawData const& rawdata);
    ub_Trigger_ChannelData_v6 ( ub_Trigger_ChannelData_v6 && ) = default;
};



}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
