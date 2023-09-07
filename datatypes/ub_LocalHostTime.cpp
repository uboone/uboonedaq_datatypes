#include "ub_LocalHostTime.h"
#include "uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

ub_LocalHostTime::ub_LocalHostTime (uint32_t sec, uint32_t usec)
: seb_time_sec(sec),seb_time_usec(usec) {  }

    
std::string ub_LocalHostTime::debugInfo()const noexcept{
      std::ostringstream os;
      os << "Object " << demangle(typeid(this)) << ".";
      os << "\n Localhost Time: (sec,usec) " << (int) seb_time_sec << ", " << (int) seb_time_usec;   
      return os.str();
}

bool ub_LocalHostTime::wasSet() const noexcept{
  return (seb_time_sec != 0 || seb_time_usec !=0 );
}
void HasLocalHostTime::copyOut(ub_LocalHostTime& target) noexcept  {
update(); target=_myValue; 
}

void HasLocalHostTime::copyIn(ub_LocalHostTime const& source) noexcept {
_myValue=source;
}
  
HasLocalHostTime& HasLocalHostTime::update() noexcept {
     struct timeval t_end;        
   //update crate header   
   gettimeofday(&t_end,NULL);
   _myValue.seb_time_sec=t_end.tv_sec;
   _myValue.seb_time_usec=t_end.tv_usec;
   return *this;
}
  
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}



