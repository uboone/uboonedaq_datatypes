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


/** EVENT Control Words. These should be at begin and end of each crate.
Header word format: (should be 0xffffffff)
Trailer word format: (should be 0xe0000000)
**/
typedef struct crate_header_v6
{
  uint32_t header;
}xmit_header_t_v6;

typedef struct crate_trailer_v6
{
  uint32_t trailer;
}xmit_trailer_t_v6;

class ub_MarkedRawCrateData_v6 : public ub_MarkedRawCrateData{

 public:
  ub_MarkedRawCrateData_v6(ub_RawData_t const& rawdata);

  ub_RawData_t const& data() const { return _marked_raw_data_block.data(); }
  ub_RawData_t const& rawdata(); const { return _marked_raw_data_block.rawdata(); }

  uint32_t const& getHeaderWord() const {return _marked_raw_data_block.header().header; } 
  uint32_t const& getTrailerWord() const {return _marked_raw_data_block.trailer().trailer; }

 private:
  ub_MarkedRawDataBlock<xmit_header_t_v6,xmit_trailer_t_v6> _marked_raw_data_block;
};



}
}
}
}

#endif
