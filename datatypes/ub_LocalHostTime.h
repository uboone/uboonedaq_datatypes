#ifndef _UBOONE_DATATYPES_LOCALHOST_TIME_H
#define _UBOONE_DATATYPES_LOCALHOST_TIME_H  1

#include <map>
#include <cstring>
#include <sys/time.h>

#include "boostSerialization.h"
#include "constants.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;


class ub_LocalHostTime final
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        if(version>0)
            ar & seb_time_sec & seb_time_usec;
    }

public:    
    ub_LocalHostTime (uint32_t sec, uint32_t usec);
    std::string debugInfo()const noexcept;
    bool wasSet() const noexcept;

public:
    uint32_t seb_time_sec=0;  // Read time on SEB. Added v4. Seconds since the epoch.
    uint32_t seb_time_usec=0; //                             Microseconds since the second      
};

struct HasLocalHostTime
{
  void copyOut(ub_LocalHostTime& target) noexcept;
  void copyIn(ub_LocalHostTime const& source) noexcept;
  
  HasLocalHostTime& update() noexcept;  
  ub_LocalHostTime _myValue={0,0};
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_LocalHostTime, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)    

#endif //_UBOONE_DATATYPES_LOCALHOST_TIME_H

