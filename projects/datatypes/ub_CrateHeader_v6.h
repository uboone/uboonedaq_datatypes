#ifndef _UBOONE_TYPES_CRATEHEADER_H
#define _UBOONE_TYPES_CRATEHEADER_H 1

#include "uboone_data_common.h"       
#include "ub_GPS_DataTypes.h"
#include "ub_TriggerBoardClock.h"
#include "ub_TPC_CardHeader_v6.h"
#include "ub_PMT_CardHeader_v6.h"
#include "constants.h"
#include <sys/time.h>
#include "boostSerialization.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

struct ub_CrateHeader_v6 final
{
    union {
        ub_fragment_header data_transmission_header; //must be first element
        uint8_t ub_fragment_header_raw_bytes[ub_fragment_header_size];
    } ;

    bool     complete; // 1 if sub-event is guaranteed complete, 0 otherwise.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"    
    union {
       struct {
            uint16_t number             :4; //bits [0-3] = 4 bits for crate 0 through 9
            uint16_t type               :4; //bits [4-7] = 4 bits for crate type (PMT/TPC)
            uint32_t reserved           :8; //bits [8-f] = 8 bits currently open
    };
    uint16_t crateBits; // word=fedcba9876543210 ... bits [0-3] = 4 bits for crate 0 through 9
    //                           bits [4-7] = 4 bits for crate type (PMT/TPC)
    //                           bits [8-f] = 8 bits currently open
    };
#pragma GCC diagnostic pop

    std::size_t size; //bytes, needs to be uint32_t for large events
    uint8_t crate_number; // Crate #
    uint8_t card_count; // Card count
    uint8_t crate_type; // crate type
    uint32_t event_number; // Event #
    uint32_t frame_number; // Frame #
    //time
    ub_GPS_Time          gps_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS time
    ub_TriggerBoardClock trigger_board_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS frame/sample/div
    ub_LocalHostTime     local_host_time; 


    ub_CrateHeader_v6();
    ub_CrateHeader_v6(ub_TPC_CardHeader_v6 const& cardHeader);
    ub_CrateHeader_v6(ub_PMT_CardHeader_v6 const& cardHeader);
    std::string debugInfo()const noexcept;

    void updateDTHeader(ub_RawData const& data)  throw(datatypes_exception);
    void updateCrateBits() noexcept;

    uint64_t sequenceID() const noexcept;
    bool compare(ub_CrateHeader_v6 const&,bool do_rethrow=false) const throw(datatypes_exception);
    static ub_CrateHeader_v6 const& getHeaderFromFragment(ub_RawData const& data) throw(datatypes_exception);
    
    void copyIn(ub_CrateHeader_v6 const& source)  noexcept;
    void copyOut(ub_CrateHeader_v6&  target)  noexcept;

};




}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif //_UBOONE_TYPES_CRATEHEADER_H



