#include "ub_TriggerBoardClock.h"
#include "uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


ub_TriggerBoardClock::ub_TriggerBoardClock (uint32_t f, uint16_t s, uint16_t d)
: frame(f),sample(s),div(d) {}

bool ub_TriggerBoardClock::operator<(const ub_TriggerBoardClock& mk) const {
        return (frame < mk.frame);
}
    
std::string ub_TriggerBoardClock::debugInfo()const noexcept{
      std::ostringstream os;
      os << "Object " << demangle(typeid(this)) << ".";
      os << "\n Trigger Clock: (frame,sample,div) " << (int) frame << ", " << (int) sample << ", " << (int) div ;   
      return os.str();
}
    
bool ub_TriggerBoardClock::wasSet() const noexcept {
return (frame!=0 || sample!=0 || div!=0 );
}


void HasTriggerBoardClock::copyOut(ub_TriggerBoardClock& target) noexcept  { 
target=_myValue; 
}

void HasTriggerBoardClock::copyIn(ub_TriggerBoardClock const& source) noexcept {
_myValue=source;
}  

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
