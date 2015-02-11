#ifndef _UBOONE_TYPES_CRATEHEADER_H
#define _UBOONE_TYPES_CRATEHEADER_H 1

#include "ub_GPS_DataTypes.h"
#include "ub_TriggerBoardClock.h"
#include "ub_TPC_CardHeader_v6.h"
#include "ub_PMT_CardHeader_v6.h"
#include "constants.h"

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
  
/**
   This header is created at seb, with crate-level information to be sent to assembler.
 **/
struct ub_CrateHeader_v6
{ 
  union {
    ub_fragment_header data_transmission_header; //must be first element
    uint8_t ub_fragment_header_raw_bytes[ub_fragment_header_size];
  } ;
  
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
 
  ub_CrateHeader_v6():
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{0},
  frame_number{0},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}
  
  ub_CrateHeader_v6(ub_TPC_CardHeader_v6 cardHeader):
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{cardHeader.getEvent()},
  frame_number{cardHeader.getFrame()},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}

  ub_CrateHeader_v6(ub_PMT_CardHeader_v6 cardHeader):
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{cardHeader.getEvent()},
  frame_number{cardHeader.getFrame()},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}
  

   template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {    
      ar & ub_fragment_header_raw_bytes;
     
      if(version > 0)
      {
      ar & complete;
      ar & crateBits; // word=fedcba9876543210 ... bits [0-3] = 4 bits for crate 0 through 9
                      //                           bits [4-7] = 4 bits for crate type (PMT/TPC)
                      //                           bits [8-f] = 8 bits currently open
      ar &  size; //bytes, needs to be uint32_t for large events
      ar &  crate_number; // Crate #
      ar &  card_count; // Card count
      ar &  event_number; // Event #
      ar &  frame_number; // Frame #
      ar &  gps_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS time
      ar &  daqClock_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS frame/sample/div  
      ar &  seb_time_sec;  // Read time on SEB. Added v4. Seconds since the epoch.
      ar &  seb_time_usec; //                             Microseconds since the second 
      }
    }
};
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_CrateHeader_v6, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif //_UBOONE_TYPES_CRATEHEADER_H



