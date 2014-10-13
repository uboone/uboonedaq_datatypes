#ifndef _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H 1

#include "uboonetypes_Base.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_ChannelData;
class ub_MarkedRawChannelData;

class ub_ChannelDataCreatorHelperClass{
 public:
  ub_ChannelDataCreatorHelperClass(ub_VersionWord_t const, ub_RawData_t const);
  std::vector<ub_ChannelData> getChannelDataVector() { return _channelDataVector; }

 private:
  std::vector<ub_ChannelData> _channelDataVector;
  std::unique_ptr<ub_MarkedRawChannelData> _markedRawChannelData;

};


}
}
}
}

#endif
