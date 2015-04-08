#include "ub_BeamHeader.h"

using namespace gov::fnal::uboone::datatypes;

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamHeader &bh) {
    return os <<"Record type: " << (int)bh.getRecordType() << std::endl
           <<"Event signal: "<< bh.getEventSignal() << std::endl
           <<"Seconds: "<< bh.getSeconds() << std::endl
           <<"Milli seconds: "<< bh.getMilliSeconds() << std::endl
           <<"Number of devices: "<< (int)bh.getNumberOfDevices() << std::endl
           <<"Number of bytes in record: "<< bh.getNumberOfBytesInRecord() << std::endl;
}

ub_BeamHeader::ub_BeamHeader():
	    record_type {0},
            event_signal {""},
            seconds {0},
            milli_seconds {0},
            number_of_devices {0},
	    number_of_bytes_in_record {0} {
}

uint8_t ub_BeamHeader::getRecordType() const noexcept {
    return record_type;
}

std::string ub_BeamHeader::getEventSignal() const noexcept {
    return event_signal;
}
uint32_t ub_BeamHeader::getSeconds() const noexcept {
    return seconds;
}
uint16_t ub_BeamHeader::getMilliSeconds() const noexcept {
    return milli_seconds;
}
uint16_t ub_BeamHeader::getNumberOfDevices() const noexcept {
    return number_of_devices;
}
uint32_t ub_BeamHeader::getNumberOfBytesInRecord() const noexcept {
    return number_of_bytes_in_record;
}

void ub_BeamHeader::setRecordType(uint8_t const& val) noexcept {
    record_type=val;
}
void ub_BeamHeader::setEventSignal(std::string const& val) noexcept {
    event_signal=val;
}
void ub_BeamHeader::setSeconds(uint32_t const& val) noexcept {
    seconds=val;
}
void ub_BeamHeader::setMilliSeconds(uint16_t const& val) noexcept {
    milli_seconds=val;
}
void ub_BeamHeader::setNumberOfDevices(uint16_t const& val) noexcept {
    number_of_devices=val;
}
void ub_BeamHeader::setNumberOfBytesInRecord(uint32_t const& val) noexcept {
    number_of_bytes_in_record=val;
}

bool ub_BeamHeader::operator<(ub_BeamHeader const& h ) const noexcept {
    return ((seconds < h.seconds) || (seconds == h.seconds && milli_seconds<h.milli_seconds))  ;
}
bool ub_BeamHeader::operator<=(ub_BeamHeader const& h ) const noexcept {
    return ((seconds < h.seconds) || (seconds == h.seconds && milli_seconds<=h.milli_seconds))  ;
}
