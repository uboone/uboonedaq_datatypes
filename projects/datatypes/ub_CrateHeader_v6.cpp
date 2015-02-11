#include "ub_CrateHeader_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

  ub_CrateHeader_v6::ub_CrateHeader_v6():
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{0},
  frame_number{0},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}
  
  ub_CrateHeader_v6::ub_CrateHeader_v6(ub_TPC_CardHeader_v6 cardHeader):
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{cardHeader.getEvent()},
  frame_number{cardHeader.getFrame()},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}

  ub_CrateHeader_v6::ub_CrateHeader_v6(ub_PMT_CardHeader_v6 cardHeader):
  data_transmission_header{},
  complete{0},crateBits{0},
  size{0},
  crate_number{0},
  card_count{0},
  event_number{cardHeader.getEvent()},
  frame_number{cardHeader.getFrame()},
  gps_time{},
  daqClock_time{},
  seb_time_sec{0},
  seb_time_usec{0}
  {}
  
std::string ub_CrateHeader_v6::debugInfo()const
{
	std::ostringstream os;
	
	os << "Object " << demangle(typeid(this)) << "."<< std::endl;
	os << "\n Event Number " << event_number;
	os << "  Frame Number " <<  frame_number;
	os << "  Crate Number " << (unsigned int)crate_number;
	os << "  Crate Bits " << (unsigned int) crateBits;
	os << "  Crate Complete " << (unsigned int) complete;
	os << "  Crate seb_time_sec " << (unsigned int)seb_time_sec;
	os << "  Crate seb_time_usec " << (unsigned int) seb_time_usec;
	if(crate_number == 10){
	  os << "  \n Crate 10 gps_time (sec, micro, nano) " << gps_time.second << ",  " 
	     << gps_time.micro << ",  " << gps_time.nano ;	
	  os << "  \n Crate 10 daqClockTime (frame, sample, div) " << (unsigned int) daqClock_time.frame
	     << ", " << (unsigned int) daqClock_time.sample << ", " << (unsigned int)daqClock_time.div;
	}
	os << "\n  Event Size " << size;
	
	return os.str();
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 




