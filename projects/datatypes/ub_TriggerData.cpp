
#include <time.h>
#include "ub_TriggerData.h"

using namespace gov::fnal::uboone::datatypes;

ub_TriggerData::ub_TriggerData() {

    bt_trigger_data.word1 = 0;
    bt_trigger_data.word2 = 0;
    bt_trigger_data.word3 = 0;
    bt_trigger_data.word4 = 0;
    bt_trigger_data.word5 = 0;
    bt_trigger_data.word6 = 0;
    bt_trigger_data.word7 = 0;
    bt_trigger_data.word8 = 0;

}

ub_TriggerData::ub_TriggerData(trigger_data_t const& bt) {
    bt_trigger_data = bt;
}

// Decoding notes:
// See docdb 2655 by Georgia.
// word1 here corresponds to word 1R in the docdb
// word2 here correspnods to word 1L in the docdb
// word3 here corresponds to word 2R in the docdb etc.
//

uint16_t ub_TriggerData::getSampleNumber() const noexcept {
    return (bt_trigger_data.word1>>4);
}
uint16_t ub_TriggerData::getSampleRemainder() const noexcept {
    return ((bt_trigger_data.word1 >> 1)&0x7);
}
uint16_t ub_TriggerData::getSampleNumber_2MHz() const noexcept {
    return (bt_trigger_data.word1>>4);
}
uint16_t ub_TriggerData::getSampleNumber_16MHz() const noexcept {
    return (bt_trigger_data.word1>>1);
}
uint16_t ub_TriggerData::getSampleNumber_64MHz() const noexcept {
    return (getSampleNumber_16MHz()<<2) + ((bt_trigger_data.word6 & 0x1)<<1) + (bt_trigger_data.word5>>15);
}

bool     ub_TriggerData::getBusy() const noexcept {
    return (bt_trigger_data.word1 &0x1);
}
uint32_t ub_TriggerData::getFrame() const noexcept {
    return (bt_trigger_data.word2 + ((bt_trigger_data.word3&0xFF)<<16));
}
uint32_t ub_TriggerData::getTrigEventNum() const noexcept {
    return ((bt_trigger_data.word3>>8) + ((bt_trigger_data.word4<<8)));
}
uint16_t  ub_TriggerData::getTriggerBits() const noexcept {
    return (bt_trigger_data.word5 & 0x7FFF);
}

// Interrogate trigger bits
// These could in principle change during commissioning or if a major change is made to the trigger; be wary.
bool     ub_TriggerData::isPmtTrigger()    const noexcept {
    return (bt_trigger_data.word5 & 0xFF)!=0;    // Will be further refined..?
}
bool     ub_TriggerData::isExtTrigger()    const noexcept {
    return (bt_trigger_data.word5 & 0x100)!=0;
}
bool     ub_TriggerData::isActiveTrigger() const noexcept {
    return (bt_trigger_data.word5 & 0x200)!=0;
}
bool     ub_TriggerData::isBnbTrigger()    const noexcept {
    return (bt_trigger_data.word5 & 0x400)!=0;
}
bool     ub_TriggerData::isNumiTrigger()   const noexcept {
    return (bt_trigger_data.word5 & 0x800)!=0;
}
bool     ub_TriggerData::isVetoTrigger()   const noexcept {
    return (bt_trigger_data.word5 & 0x1000)!=0;
}
bool     ub_TriggerData::isCalibTrigger()  const noexcept {
    return (bt_trigger_data.word5 & 0x2000)!=0;
}

uint16_t ub_TriggerData::getPhase64Mhz_1()    const noexcept {
    return (bt_trigger_data.word5>>15);
}
uint16_t ub_TriggerData::getPhase64Mhz_2()    const noexcept {
    return (bt_trigger_data.word6 & 0x1);
}

// Synonyms for back-compatability.
uint16_t ub_TriggerData::getTrigEventType()   const noexcept {
    return (bt_trigger_data.word5 & 0x7FFF);
}
uint64_t ub_TriggerData::getClock()           const noexcept {
    return (bt_trigger_data.word1>>4);
}

