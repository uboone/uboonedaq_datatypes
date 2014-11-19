#include "ub_TPC_CrateData_v6.h"

using namespace gov::fnal::uboone::datatypes;	

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <>
bool  ub_MarkedRawCrateData<ub_TPC_CardData_v6>::isValid()
{
 	//FIXME: GAL add additional validation for the header and trailer		
	return true;
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

