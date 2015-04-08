#include "ub_GlobalHeader.h"


using namespace gov::fnal::uboone::datatypes;

ub_GlobalHeader::ub_GlobalHeader()
   :record_type {RESERVED},
    record_origin {0xff},
    event_type {UNUSED_TYPE},
    run_number {0xffffffff},
    subrun_number {0xffffffff},
    event_number {0xffffffff},
    event_number_crate {0xffffffff},
    seconds {0xffffffff},
    milli_seconds {0xffff},
    micro_seconds {0xffff},
    nano_seconds {0xffff},
    numberOfBytesInRecord {0},
    number_of_sebs {0} {
}

void ub_GlobalHeader::setRecordType(uint8_t const& type) noexcept {
    record_type = type;
}
void ub_GlobalHeader::setRecordOrigin(uint8_t const& origin) noexcept {
    record_origin = origin;
}
void ub_GlobalHeader::setEventType(uint8_t const& type) noexcept {
    event_type = type;
}
void ub_GlobalHeader::setRunNumber(uint32_t const& run) noexcept {
    run_number = run;
}
void ub_GlobalHeader::setSubrunNumber(uint32_t const& subrun) noexcept {
    subrun_number = subrun;
}
void ub_GlobalHeader::setEventNumber(uint32_t const& event) noexcept {
    event_number = event;
}
void ub_GlobalHeader::setEventNumberCrate(uint32_t const& event) noexcept {
    event_number_crate = event;
}
void ub_GlobalHeader::setSeconds(uint32_t const& s) noexcept {
    seconds = s;
}
void ub_GlobalHeader::setMilliSeconds(uint16_t const& ms) noexcept {
    milli_seconds = ms;
}
void ub_GlobalHeader::setMicroSeconds(uint16_t const& us) noexcept {
    micro_seconds = us;
}
void ub_GlobalHeader::setNanoSeconds(uint16_t const& ns) noexcept {
    nano_seconds = ns;
}
void ub_GlobalHeader::setNumberOfBytesInRecord(uint32_t const& size) noexcept{
    numberOfBytesInRecord = size;
}
void ub_GlobalHeader::setNumberOfSEBs(uint8_t const&s) noexcept {
    number_of_sebs = s;
}

uint8_t ub_GlobalHeader::getRecordType() const noexcept {
    return record_type;
}
uint8_t ub_GlobalHeader::getRecordOrigin() const noexcept {
    return record_origin;
}
uint8_t ub_GlobalHeader::getEventType() const noexcept {
    return event_type;
}
uint32_t ub_GlobalHeader::getRunNumber() const noexcept {
    return run_number;
}
uint32_t ub_GlobalHeader::getSubrunNumber() const noexcept {
    return subrun_number;
}
uint32_t ub_GlobalHeader::getEventNumber() const noexcept {
    return event_number;
}
uint32_t ub_GlobalHeader::getEventNumberCrate() const noexcept {
    return event_number_crate;
}
uint32_t ub_GlobalHeader::getSeconds() const noexcept {
    return seconds;
}
uint16_t ub_GlobalHeader::getMilliSeconds() const noexcept {
    return milli_seconds;
}
uint16_t ub_GlobalHeader::getMicroSeconds() const noexcept {
    return micro_seconds;
}
uint16_t ub_GlobalHeader::getNanoSeconds() const noexcept {
    return nano_seconds;
}
uint32_t ub_GlobalHeader::getNumberOfBytesInRecord() const noexcept {
    return numberOfBytesInRecord;
}
uint8_t ub_GlobalHeader::getNumberOfSEBs() const noexcept {
    return number_of_sebs;
}

