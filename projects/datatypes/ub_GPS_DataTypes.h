#ifndef _UBOONE_TYPES_GPSDDATA_H
#define _UBOONE_TYPES_GPSDDATA_H 1

#include <cstdint>

#include "boostSerialization.h"

#include "constants.h"

/* The gps structure is what is read off the card
 * in the trigger crate and represents the GPS time at the arrival of a PPS.
 */
namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

struct ub_GPS {
    friend class boost::serialization::access;
  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if( version > 0 )
        ar & lower & upper;
    }    


    //public members
    uint32_t lower;
    uint32_t upper;

    uint32_t getLower() const noexcept;
    uint32_t getUpper() const noexcept;
    void setLower(uint32_t const& lower_) noexcept;
    void setUpper(uint32_t const& upper_) noexcept;
};


/**
   This replaces tmub ../gps/symm.h
 **/
struct ub_GPS_Time
{
    uint32_t second; // seconds since the epoch.
    uint32_t micro;  // microseconds since the second.
    uint32_t nano;  // nanoseconds since the second.   
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GPS, gov::fnal::uboone::datatypes::constants::VERSION)    


#endif //_UBOONE_TYPES_GPSDDATA_H



