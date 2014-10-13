#ifndef _UBOONE_TYPES_CHANNEL_H
#define _UBOONE_TYPES_CHANNEL_H 1

#include "uboonetypes_Base.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_MarkedRawChannelData{
 public:

  static ub_MarkedRawChannelData* CreateMarkedRawChannelData( ub_VersionWord_t const&, ub_RawData_t const& rawdata);

  virtual ub_RawData_t const& data() const;
  virtual ub_RawData_t const& rawdata(); const;

  virutal uint16_t const& getChannelHeaderWord();
  virutal uint16_t const& getChannelTrailerWord();

  virtual uint16_t getChannelNumber() const;
};

}
}
}
}

#endif
