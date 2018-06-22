#ifndef _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<typename MRCD>
class ub_ChannelDataCreatorHelperClass final{
public:
    ub_ChannelDataCreatorHelperClass(ub_RawData const& rd) :_rawData(rd) {}
    void populateChannelDataVector(std::vector<MRCD> & channelDataVector) ;
private:
    const ub_RawData _rawData;
};
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H
