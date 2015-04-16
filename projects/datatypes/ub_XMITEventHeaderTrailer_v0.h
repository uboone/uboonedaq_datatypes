#ifndef _UBOONE_TYPES_CRATEHEADERTRAILER_H
#define _UBOONE_TYPES_CRATEHEADERTRAILER_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct ub_XMITEventHeader final
{
    union {
        struct {
            uint32_t marker;
        };
        uint32_t raw_data=0xDEADBEEF;
    };
    uint32_t getMarker() const noexcept{
        return marker;
    }
    std::string debugInfo()const noexcept;
};


static_assert (sizeof(ub_XMITEventHeader) == 4, "ub_XMITEventHeader structure size is not correct.");

struct ub_XMITEventTrailer final
{
    union {
        struct {
            uint32_t marker;
        };
        uint32_t raw_data=0xDEADBEEF;
    };
    uint32_t getMarker() const noexcept{
        return marker;
    }
    std::string debugInfo()const noexcept;
};

static_assert (sizeof(ub_XMITEventTrailer) == 4, "ub_XMITEventTrailer structure size is not correct.");
#pragma GCC diagnostic pop

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CRATEHEADERTRAILER_H

