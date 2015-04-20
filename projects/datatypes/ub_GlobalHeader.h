#ifndef _UBOONETYPES_GLOBALHEADER_H
#define _UBOONETYPES_GLOBALHEADER_H

#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"

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
            & seconds & milli_seconds & micro_seconds & nano_seconds
            & numberOfBytesInRecord & number_of_sebs & is_event_complete;
    }
    
public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
    ub_GlobalHeader();

    void setRecordType(uint8_t const& type) noexcept;
    void setRecordOrigin(uint8_t const& origin) noexcept;
    void setEventType(uint8_t const& type) noexcept;
    void setRunNumber(uint32_t const& run) noexcept;
    void setSubrunNumber(uint32_t const& subrun) noexcept;
    void setEventNumber(uint32_t const& event) noexcept;
    void setEventNumberCrate(uint32_t const& event) noexcept;
    void setSeconds(uint32_t const& s) noexcept;
    void setMilliSeconds(uint16_t const& ms) noexcept;
    void setMicroSeconds(uint16_t const& us) noexcept;
    void setNanoSeconds(uint16_t const& ns) noexcept;
    void setNumberOfBytesInRecord(uint32_t const& size) noexcept;
    void setNumberOfSEBs(uint8_t const& s) noexcept;
    void markIncomplete() noexcept;
    void markComplete() noexcept;

    uint8_t getRecordType() const noexcept;
    uint8_t getRecordOrigin() const noexcept;
    uint8_t getEventType() const noexcept;
    uint32_t getRunNumber() const noexcept;
    uint32_t getSubrunNumber() const noexcept;
    uint32_t getEventNumber() const noexcept;
    uint32_t getEventNumberCrate() const noexcept;
    uint32_t getSeconds() const noexcept;
    uint16_t getMilliSeconds() const noexcept;
    uint16_t getMicroSeconds() const noexcept;
    uint16_t getNanoSeconds() const noexcept;
    uint32_t getNumberOfBytesInRecord() const noexcept;
    uint8_t getNumberOfSEBs() const noexcept;
    bool isComplete() const noexcept;

    std::string debugInfo()const noexcept;

private:
    uint8_t record_type;   /* From event_types.h */
    uint8_t record_origin; /* DATA or MC */
    uint8_t event_type;
    uint32_t run_number;
    uint32_t subrun_number;
    uint32_t event_number;
    uint32_t event_number_crate; /* Crate's sense of the evt #. */

    uint32_t seconds; // GPS clock. Since Jan 1, 2012.
    uint16_t milli_seconds;
    uint16_t micro_seconds;
    uint16_t nano_seconds;
    uint32_t numberOfBytesInRecord;

    uint8_t number_of_sebs;
    uint8_t is_event_complete;

};

}  // end of namespace sebs
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_GlobalHeader, gov::fnal::uboone::datatypes::constants::VERSION)

#endif /* #ifndef BOONETYPES_H */



