#ifndef _UBOONE_TYPES_CRATE_H
#define _UBOONE_TYPES_CRATE_H 1

#include "uboonetypes_Base.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_MarkedRawCrateData{
 public:

  static ub_MarkedRawCrateData* CreateMarkedRawCrateData( ub_VersionWord_t const&, ub_RawData_t const& rawdata);

  virtual ub_RawData_t const& data() const;
  virtual ub_RawData_t const& rawdata(); const;

  virtual uint32_t const& getHeaderWord() const; 
  virtual uint32_t const& getTrailerWord() const;

};

}
}
}
}

#endif
