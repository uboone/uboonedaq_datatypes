#ifndef _UBOONE_DATATYPES_EMPTYHEADER_H
#define _UBOONE_DATATYPES_EMPTYHEADER_H  1

#include "uboone_data_utils.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
struct empty_header final{
    uint16_t foo; //unused
    std::string debugInfo()const noexcept;
};

template <> constexpr
size_t size_of<empty_header>() {
    return 0;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_DATATYPES_EMPTYHEADER_H

