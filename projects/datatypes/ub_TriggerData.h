#ifndef _UBOONETYPES_TRIGGERDATA_H
#define _UBOONETYPES_TRIGGERDATA_H

#include "boostSerialization.h"
#include "constants.h"
#include "evttypes.h"
#include "ub_Trigger_DataTypes.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

/**
   Note: this is the serialization class for the otherwise hardcoded trigger_data_t struct,
   located in share/boonetypes. IF changes are made to trigger_data_t, the appropriate changes
   should be made here as well, and the version number should be increased.
 **/

class ub_TriggerData {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        if( version >0 )
            ar & bt_trigger_data.word1 & bt_trigger_data.word2 & bt_trigger_data.word3 & bt_trigger_data.word4
            & bt_trigger_data.word5 & bt_trigger_data.word6 & bt_trigger_data.word7 & bt_trigger_data.word8;
    }

public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
    ub_TriggerData();
    ub_TriggerData(trigger_data_t const& bt);

    uint16_t getSampleNumber() const noexcept;
    uint16_t getSampleRemainder() const noexcept;
    uint16_t getSampleNumber_2MHz() const noexcept;
    uint16_t getSampleNumber_16MHz() const noexcept;
    uint16_t getSampleNumber_64MHz() const noexcept;
    bool     getBusy() const noexcept;
    uint32_t getFrame() const noexcept;
    uint32_t getTrigEventNum() const noexcept;
    uint16_t  getTriggerBits() const noexcept;
    bool     isPmtTrigger() const noexcept;
    bool     isExtTrigger() const noexcept;
    bool     isActiveTrigger() const noexcept;
    bool     isBnbTrigger()    const noexcept;
    bool     isNumiTrigger()   const noexcept;
    bool     isVetoTrigger()   const noexcept;
    bool     isCalibTrigger()  const noexcept;
    uint16_t getPhase64Mhz_1() const noexcept;
    uint16_t getPhase64Mhz_2() const noexcept;

    // Synonyms for back-compatability.
    uint16_t getTrigEventType() const noexcept;
    uint64_t getClock()         const noexcept;
    
private:
    trigger_data_t bt_trigger_data;
};


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_TriggerData, gov::fnal::uboone::datatypes::constants::VERSION)

#endif /* #ifndef BOONETYPES_H */



