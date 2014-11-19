#ifndef _UBOONE_TYPES_MARKEDTPCCRATEDATA_V6_H
#define _UBOONE_TYPES_MARKEDTPCCRATEDATA_V6_H 1


#include "ub_MarkedRawCrateData.h"

#include "ub_TPC_CardData_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
	
  class ub_TPC_CrateData_v6 : public ub_MarkedRawCrateData<ub_TPC_CardData_v6>{
  public:    
	ub_TPC_CrateData_v6(ub_RawData const rawdata): 
	ub_MarkedRawCrateData<ub_TPC_CardData_v6>(rawdata){}

	ub_TPC_CrateData_v6() = delete;
//	ub_TPC_CrateData_v6(ub_TPC_CrateData_v6 const &) = delete;
	ub_TPC_CrateData_v6& operator=(ub_TPC_CrateData_v6 const &) = delete;
};
 
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_TYPES_MARKEDTPCCRATEDATA_V6_H

