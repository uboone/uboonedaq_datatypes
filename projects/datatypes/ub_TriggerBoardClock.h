#ifndef _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H
#define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  1

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
  
// This struct will be a key in a map, so I must define "<".
struct ub_TriggerBoardClock
{
  uint32_t frame;
  uint16_t sample;
  uint16_t div;

    ub_TriggerBoardClock (uint32_t f=0, uint16_t s=0, uint16_t d=0)
    : frame(f),sample(s),div(d) {  }
  
    bool operator<(const ub_TriggerBoardClock& mk) const {
        return (frame < mk.frame);       
    }
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 
#endif // #define _UBOONE_TYPES_TRIGBOARDCLOCK_UB_H  
