#ifndef _UBOONE_TYPES_CRATEHEADER_H
#define _UBOONE_TYPES_CRATEHEADER_H 1

#include "ub_GPS_DataTypes.h"
#include "ub_TriggerBoardClock.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
  
/**
   This header is created at seb, with crate-level information to be sent to assembler.
 **/
struct ub_CrateHeader_v6
{
  bool     complete; // 1 if sub-event is guaranteed complete, 0 otherwise.
  uint16_t crateBits; // word=fedcba9876543210 ... bits [0-3] = 4 bits for crate 0 through 9
                      //                           bits [4-7] = 4 bits for crate type (PMT/TPC)
                      //                           bits [8-f] = 8 bits currently open
  uint32_t size; //bytes, needs to be uint32_t for large events
  uint8_t crate_number; // Crate #
  uint8_t card_count; // Card count
  uint32_t event_number; // Event #
  uint32_t frame_number; // Frame #
  ub_GPS_Time gps_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS time
  ub_TriggerBoardClock daqClock_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS frame/sample/div  
  uint32_t seb_time_sec;  // Read time on SEB. Added v4. Seconds since the epoch.
  uint32_t seb_time_usec; //                             Microseconds since the second
  
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){}
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 
#endif //_UBOONE_TYPES_CRATEHEADER_H



