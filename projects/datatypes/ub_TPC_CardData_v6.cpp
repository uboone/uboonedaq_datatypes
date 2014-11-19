#include "ub_TPC_CardData_v6.h"

using namespace gov::fnal::uboone::datatypes;	

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
	
	template<>
	bool ub_MarkedRawCardData<ub_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer>::isValid()
	{
		//FIXME: GAL add additional validation for the header and trailer		
		return true;
	}
	
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 
  
uint32_t ub_TPC_CardData_v6::getTrigFrame() const
{
  // Attempt to resolve the rollover situation: the lower 4 bits are given by the trigger frame, which should crudely match the upper bits from the course frame. 
  // Here is how I resolved it --- Nathaniel
  uint32_t frameCourse = getFrame();
  uint32_t option1 = (getFrame()&0xFFFFFFF0) | (getTrigFrameMod16());
  int32_t diff = option1-frameCourse;
  if(diff > 8) return option1 - 0x10; // Solution is too high; rollover down
  if(diff < 8) return option1 + 0x10; // Solution is too low; rollover up.
  return option1; // if within 8 ticks, this solution is fine.
}


	