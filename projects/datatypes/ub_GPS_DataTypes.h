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

struct ub_GPS final{
    friend class boost::serialization::access;
  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if( version > 0 )
        ar & lower & upper;
    }    


    //public members
    uint32_t lower=0;
    uint32_t upper=0;

    uint32_t getLower() const noexcept;
    uint32_t getUpper() const noexcept;
    void setLower(uint32_t const& lower_) noexcept;
    void setUpper(uint32_t const& upper_) noexcept;
    
    std::string debugInfo()const noexcept;
};


/**
   This replaces tmub ../gps/symm.h
 **/
class ub_GPS_Time final
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        if(version>0)
            ar & second & micro & nano;
    }

public:
    ub_GPS_Time (uint32_t sec, uint32_t usec, uint32_t nano_sec);

    std::string debugInfo()const noexcept;
    bool wasSet() const noexcept;
public:
    uint32_t second; // seconds since the epoch.
    uint32_t micro;  // microseconds since the second.
    uint32_t nano;  // nanoseconds since the second.  
    
};


struct HasGPSTime 
{
  void copyOut(ub_GPS_Time& target) noexcept;
  void copyIn(ub_GPS_Time const& source) noexcept;
  ub_GPS_Time _myValue ={0,0,0};
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GPS_Time, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)    
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GPS, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)    

#endif //_UBOONE_TYPES_GPSDDATA_H



