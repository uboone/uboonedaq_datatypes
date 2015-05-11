#ifndef _UBOONETYPES_GLOBALHEADER_H
#define _UBOONETYPES_GLOBALHEADER_H

#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"
#include "ub_LocalHostTime.h"
#include "ub_GPS_DataTypes.h"
#include "ub_TriggerBoardClock.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

/***
 *  Global header is attached to front of each event by assembler,
 *  in ProcessAndShipEventReader thread.
 ***/


class ub_GlobalHeader final{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        if(version>0)
            ar & record_type & record_origin & event_type
            & run_number & subrun_number & event_number & event_number_crate
            & numberOfBytesInRecord & number_of_sebs & is_event_complete 
            & local_host_time & trigger_board_time & gps_time;            
    }
    
public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;
    ub_GlobalHeader();

    void setRecordType(uint8_t const& type) noexcept;
    void setRecordOrigin(uint8_t const& origin) noexcept;
    void setEventType(uint8_t const& type) noexcept;
    void setRunNumber(uint32_t const& run) noexcept;
    void setSubrunNumber(uint32_t const& subrun) noexcept;
    void setEventNumber(uint32_t const& event) noexcept;
    void setEventNumberCrate(uint32_t const& event) noexcept;
    void setNumberOfBytesInRecord(uint32_t const& size) noexcept;
    void setNumberOfSEBs(uint8_t const& s) noexcept;
    void markIncomplete() noexcept;
    void markComplete() noexcept;

    void setGPSTime(ub_GPS_Time const& gps) noexcept;
    void setTriggerBoardClock(ub_TriggerBoardClock const& trigger_board) noexcept;
    void setLocalHostTime(ub_LocalHostTime const& localhost) noexcept;

    
    uint8_t getRecordType() const noexcept;
    uint8_t getRecordOrigin() const noexcept;
    uint8_t getEventType() const noexcept;
    uint32_t getRunNumber() const noexcept;
    uint32_t getSubrunNumber() const noexcept;
    uint32_t getEventNumber() const noexcept;
    uint32_t getEventNumberCrate() const noexcept;
    uint32_t getSeconds() const noexcept;
    uint16_t getMicroSeconds() const noexcept;
    uint16_t getNanoSeconds() const noexcept;
    uint32_t getNumberOfBytesInRecord() const noexcept;
    uint8_t getNumberOfSEBs() const noexcept;
    
    ub_GPS_Time const& getGPSTime() const noexcept;    
    ub_TriggerBoardClock const& getTriggerBoardClock() const noexcept;
    ub_LocalHostTime const& getLocalHostTime() const noexcept;
    
    bool isComplete() const noexcept;

    std::string debugInfo()const noexcept;

private:
    ub_LocalHostTime     local_host_time; 
    ub_GPS_Time          gps_time;           // Inserted for SEB-10 only in rawFragmentDMASource.cpp:  
    ub_TriggerBoardClock trigger_board_time; // Inserted for SEB-10 only in rawFragmentDMASource.cpp: PPS frame/sample/div

    uint8_t record_type;   /* From event_types.h */
    uint8_t record_origin; /* DATA or MC */
    uint8_t event_type;
    uint32_t run_number;
    uint32_t subrun_number;
    uint32_t event_number;
    uint32_t event_number_crate; /* Crate's sense of the evt #. */

    uint32_t numberOfBytesInRecord;

    uint8_t number_of_sebs;
    uint8_t is_event_complete;
    

};

}  // end of namespace sebs
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GlobalHeader, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)

#endif /* #ifndef BOONETYPES_H */



