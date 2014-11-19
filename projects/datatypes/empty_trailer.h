#ifndef _UBOONE_DATATYPES_EMPTYTRAILER_H
#define _UBOONE_DATATYPES_EMPTYTRAILER_H  1

#include "uboone_data_utils.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {	
struct empty_trailer{ 
	uint16_t foo; //unused
	std::string debugInfo()const;
};

template <> constexpr 
size_t size_of<empty_trailer>(){
	return 0;
}


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_DATATYPES_EMPTYTRAILER_H

