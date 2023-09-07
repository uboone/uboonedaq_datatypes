#ifndef _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H
#define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  1

#include <sstream> 
#include <iostream> 

#include "constants.h"
#include "boostSerialization.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

// This struct will be a key in a map, so I must define "<".
class ub_TriggerBoardClock final
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        if(version>0)
            ar & frame & sample & div;
    }
    
public:
    ub_TriggerBoardClock (uint32_t f, uint16_t s, uint16_t d);
    bool operator<(const ub_TriggerBoardClock& mk) const;    
    std::string debugInfo()const noexcept;
    bool isValid() const noexcept;
    bool wasSet() const noexcept;

public:
    uint32_t frame=0;
    uint16_t sample=0;
    uint16_t div=0;
    
};


struct HasTriggerBoardClock
{
  void copyOut(ub_TriggerBoardClock& target) noexcept;
  void copyIn(ub_TriggerBoardClock const& source) noexcept;
  ub_TriggerBoardClock _myValue = {0,0,0};
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif // #define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  
