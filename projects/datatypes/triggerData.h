#ifndef _UBOONETYPES_TRIGGERDATA_H
#define _UBOONETYPES_TRIGGERDATA_H
#include <sys/types.h>
#include <inttypes.h>
#include "evttypes.h"
#include "share/boonetypes.h"

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/greg_serialize.hpp"
#include "boost/date_time/posix_time/time_serialize.hpp"
#include "constants.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

/**
   Note: this is the serialization class for the otherwise hardcoded trigger_data_t struct,
   located in share/boonetypes. IF changes are made to trigger_data_t, the appropriate changes
   should be made here as well, and the version number should be increased.
 **/

class triggerData {

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
  triggerData();
  triggerData(trigger_data_t bt) { bt_trigger_data = bt; }

  uint16_t getSampleNumber()    const { return (bt_trigger_data.word1>>4); }
  uint16_t getSampleRemainder() const { return ((bt_trigger_data.word1 >> 1)&0x7); }
  bool     getBusy()            const { return (bt_trigger_data.word1 &0x1); }
  uint32_t getFrame()           const { return (bt_trigger_data.word2 + ((bt_trigger_data.word3&0xFF)<<16)); }
  uint32_t getTrigEventNum()   const { return ((bt_trigger_data.word3>>8) + ((bt_trigger_data.word4<<8))); }
  uint16_t  getTriggerBits()    const { return (bt_trigger_data.word5 & 0x7FFF);}
  bool     getPhase64Mhz_1()    const { return (bt_trigger_data.word5>>15);}
  bool     getPhase64Mhz_2()    const { return (bt_trigger_data.word6 & 0x1);}

  // Synonyms for back-compatability.
  uint16_t getTrigEventType()   const { return (bt_trigger_data.word5 & 0x7FFF);}
  uint64_t getClock()           const { return (bt_trigger_data.word1>>4); }

  // void setTrigEventNum(uint32_t event) {bt_trigger_data.trig_event_num = event;}
  // void setTrigEventType(uint16_t type) {bt_trigger_data.trig_event_type = type;}
  // void setFrame(uint16_t frame) {bt_trigger_data.frame = frame;}
  // void setClock(uint64_t clock) {bt_trigger_data.clock = clock;}

 private:
  trigger_data_t bt_trigger_data;
  friend class boost::serialization::access;
  
  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if( version >0 )
	ar & bt_trigger_data.word1
	   & bt_trigger_data.word2
	   & bt_trigger_data.word3
	   & bt_trigger_data.word4
 	   & bt_trigger_data.word5
	   & bt_trigger_data.word6
	   & bt_trigger_data.word7
	   & bt_trigger_data.word8;
    }
};


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::triggerData, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif /* #ifndef BOONETYPES_H */



