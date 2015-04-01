#ifndef _UBOONE_TYPES_GPSDDATA_H
#define _UBOONE_TYPES_GPSDDATA_H 1

#include <cstdint>

#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>

#include "constants.h"

/* The gps structure is what is read off the card
 * in the trigger crate and represents the GPS time at the arrival of a PPS.
 */
namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

struct ub_GPS {
    uint32_t lower;
    uint32_t upper;

    uint32_t getLower() {
        return lower;
    }
    uint32_t getUpper() {
        return upper;
    }
    void setLower(uint32_t lower) {
        lower = lower;
    }
    void setUpper(uint32_t upper) {
        upper = upper;
    }

    
  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if( version > 0 )
        ar & lower & upper;
    }    
};


/**
   This replaces tmub ../gps/symm.h
 **/
struct ub_GPS_Time
{
    // 2^32 = 4.E9 . Thus 32 bits allows for both (2013-1970)*3.14e7 seconds and
    // enough nanoseconds to span a second.
    uint32_t second; // seconds since the epoch.
    uint32_t micro;  // microseconds since the second.
    uint32_t nano;  // nanoseconds since the second.
    ub_GPS_Time() {};
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GPS, gov::fnal::uboone::datatypes::constants::VERSION)    


#endif //_UBOONE_TYPES_GPSDDATA_H


