#ifndef _UBOONE_TYPES_TPCCHANELDATA_V6_H
#define _UBOONE_TYPES_TPCCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_ChannelData_v6 final: public ub_MarkedRawChannelData
{
public:
    ub_TPC_ChannelData_v6(ub_RawData const& rawdata);
    ub_TPC_ChannelData_v6 ( ub_TPC_ChannelData_v6 && ) = default;
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
