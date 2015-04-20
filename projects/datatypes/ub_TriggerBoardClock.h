#ifndef _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H
#define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  1

#include <sstream> 
#include <iostream> 
#include "uboone_data_utils.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

// This struct will be a key in a map, so I must define "<".
struct ub_TriggerBoardClock final
{
    uint32_t frame=0;
    uint16_t sample=0;
    uint16_t div=0;

    ub_TriggerBoardClock (uint32_t f, uint16_t s, uint16_t d)
        : frame(f),sample(s),div(d) {  }

    bool operator<(const ub_TriggerBoardClock& mk) const {
        return (frame < mk.frame);
    }
    
    std::string debugInfo()const noexcept
    {
      std::ostringstream os;
      os << "Object " << demangle(typeid(this)) << ".";
      os << "\n Trigger Clock: (frame,sample,div) " << (int) frame << ", " << (int) sample << ", " << (int) div ;   
      return os.str();
    }
};


struct HasTriggerBoardClock
{
  void copyOut(ub_TriggerBoardClock& target) noexcept  { target=_myValue; }
  void copyIn(ub_TriggerBoardClock const& source) noexcept {_myValue=source;};  
  ub_TriggerBoardClock _myValue = {0,0,0};
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif // #define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  
