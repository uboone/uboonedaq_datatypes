#ifndef _UBOONE_TYPES_CRATEHEADERTRAILER_H
#define _UBOONE_TYPES_CRATEHEADERTRAILER_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
	
struct ub_EventHeader
{
	union {
		struct{
			uint32_t marker;
		};
	uint32_t raw_data=0xDEADBEEF;
	};
	uint32_t getMarker() const {return marker;}
	std::string debugInfo()const;
};

static_assert (sizeof(ub_EventHeader) == 4, "ub_EventHeader structure size is not correct.");
  
struct ub_EventTrailer
{
	union {
		struct{
			uint32_t marker;
		};
	uint32_t raw_data=0xDEADBEEF;
	};
	uint32_t getMarker() const {return marker;}
	std::string debugInfo()const;
};
  
static_assert (sizeof(ub_EventTrailer) == 4, "ub_EventTrailer structure size is not correct.");

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CRATEHEADERTRAILER_H

