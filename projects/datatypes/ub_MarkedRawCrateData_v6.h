#ifndef _UBOONE_TYPES_MARKEDRAWCRATEDATA_V6_H
#define _UBOONE_TYPES_MARKEDRAWCRATEDATA_V6_H 1

#include "ub_MarkedRawCrateData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

  typedef struct ub_CrateHeader_v6
  {
    uint32_t header;
  }ub_CrateHeader_t_v6;
  
  typedef struct ub_CrateTrailer_v6
  {
    uint32_t trailer;
  }ub_CrateTrailer_t_v6;
  
  class ub_MarkedRawCrateData_v6 : public ub_MarkedRawCrateData{
    
  public:
    ub_MarkedRawCrateData_v6(ub_RawData_t const& rawdata);
    
    ub_RawData_t const& data() const { return _marked_raw_data_block.data(); }
    ub_RawData_t const& rawdata(); const { return _marked_raw_data_block.rawdata(); }
    
    uint32_t const& getHeaderWord() const {return _marked_raw_data_block.header().header; } 
    uint32_t const& getTrailerWord() const {return _marked_raw_data_block.trailer().trailer; }
    
  private:
    ub_MarkedRawDataBlock<ub_CrateHeader_t_v6,ub_CrateTrailer_t_v6> _marked_raw_data_block;
  };
  
  
  
}
}
}
}

#endif
