#ifndef _UBOONETYPES_BEAMHEADER_H
#define _UBOONETYPES_BEAMHEADER_H

#include "boostSerialization.h"
#include "constants.h"

#include <iostream>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

class ub_BeamHeader {
   friend class boost::serialization::access;

   template<class Archive>
     void serialize(Archive & ar, const unsigned int version){
       if(version > 0)
	 ar & record_type & event_signal & seconds & milli_seconds & number_of_devices & number_of_bytes_in_record;
     }

 public:
   ub_BeamHeader();
   
   uint8_t getRecordType() const noexcept;
   std::string getEventSignal() const noexcept;
   uint32_t getSeconds() const noexcept;
   uint16_t getMilliSeconds() const noexcept;
   uint16_t getNumberOfDevices() const noexcept;
   uint32_t getNumberOfBytesInRecord() const noexcept;

   void setRecordType(uint8_t const& val) noexcept;
   void setEventSignal(std::string const&  val) noexcept;
   void setSeconds(uint32_t const& val) noexcept;
   void setMilliSeconds(uint16_t const& val) noexcept;
   void setNumberOfDevices(uint16_t const& val) noexcept;
   void setNumberOfBytesInRecord(uint32_t const& val) noexcept;

   bool operator<(ub_BeamHeader const& h ) const noexcept;
   bool operator<=(ub_BeamHeader const& h ) const noexcept;

 private:
   uint8_t record_type;
   std::string event_signal;
   uint32_t seconds; // GPS clock. Since Jan 1, 2012. 
   uint16_t milli_seconds;
   uint16_t number_of_devices;
   uint32_t number_of_bytes_in_record;
};
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamHeader &bh);

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_BeamHeader, gov::fnal::uboone::datatypes::constants::VERSION)    


#endif /* #ifndef BOONETYPES_H */
