#include "ub_GlobalHeader.h"
#include "uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

ub_GlobalHeader::ub_GlobalHeader()
   :local_host_time{0,0},
    gps_time{0,0,0},
    trigger_board_time{0,0,0},    
    record_type {RESERVED},
    record_origin {0xff},
    event_type {UNUSED_TYPE},
    run_number {0xffffffff},
    subrun_number {0xffffffff},
    event_number {0xffffffff},
    event_number_crate {0xffffffff},
    numberOfBytesInRecord {0},
    number_of_sebs {0},
    is_event_complete(1),
    daq_version_label {""},
    daq_version_quals {""}
{
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
void ub_GlobalHeader::setNumberOfBytesInRecord(uint32_t const& size) noexcept{
    numberOfBytesInRecord = size;
}
void ub_GlobalHeader::setNumberOfSEBs(uint8_t const&s) noexcept {
    number_of_sebs = s;
}
void ub_GlobalHeader::setDAQVersionLabel(std::string const& s) noexcept {
  daq_version_label = s;
}
void ub_GlobalHeader::setDAQVersionQualifiers(std::string const& s) noexcept {
  daq_version_quals = s;
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
    return gps_time.second;
}

uint16_t ub_GlobalHeader::getMicroSeconds() const noexcept {
    return gps_time.micro;
}
uint16_t ub_GlobalHeader::getNanoSeconds() const noexcept {
    return gps_time.nano;
}
uint32_t ub_GlobalHeader::getNumberOfBytesInRecord() const noexcept {
    return numberOfBytesInRecord;
}
uint8_t ub_GlobalHeader::getNumberOfSEBs() const noexcept {
    return number_of_sebs;
}
std::string ub_GlobalHeader::getDAQVersionLabel() const noexcept {
  return daq_version_label;
}
std::string  ub_GlobalHeader::getDAQVersionQualifiers() const noexcept {
  return daq_version_quals;
}

void ub_GlobalHeader::markIncomplete() noexcept{
    is_event_complete=0;
}

void ub_GlobalHeader::markComplete() noexcept{
    is_event_complete=1;
}

bool ub_GlobalHeader::isComplete() const noexcept{
    return is_event_complete!=0;
}


void ub_GlobalHeader::setGPSTime(ub_GPS_Time const& gps) noexcept {
    gps_time = gps;
}

void ub_GlobalHeader::setTriggerBoardClock(ub_TriggerBoardClock const& trigger_board) noexcept{
    trigger_board_time= trigger_board;
}

void ub_GlobalHeader::setLocalHostTime(ub_LocalHostTime const&local_host) noexcept{
    local_host_time=local_host;
}

ub_GPS_Time const& ub_GlobalHeader::getGPSTime() const noexcept {
    return gps_time;
}

ub_TriggerBoardClock const& ub_GlobalHeader::getTriggerBoardClock() const noexcept{
    return trigger_board_time;
}

ub_LocalHostTime const& ub_GlobalHeader::getLocalHostTime() const noexcept{
    return local_host_time;
}

std::string ub_GlobalHeader::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n Event Info:" ;
    os << "\n  run_number=" << (int) run_number;
    os << "\n  subrun_number=" << (int) subrun_number;
    os << "\n  event_number=" << (int) event_number;
    os << "\n  event_number_crate=" << (int) event_number_crate;
    os << "\n  numberOfBytesInRecord=" << (int) numberOfBytesInRecord;
    os << "\n  number_of_sebs=" << (int) number_of_sebs;        
    os << "\n Event Details:" ;           
    os << " record_type=" << (int) record_type;
    os << " record_origin=" << (int) record_origin;
    os << " event_type=" << (int) event_type;

    os << "\n Event Time:" ;           
    os << " seconds=" << (int) getSeconds();
    os << " micro_seconds=" << (int) getMicroSeconds();
    os << " nano_seconds=" << (int) getNanoSeconds();
    
    os << "\n GPS Time:";
    os << gps_time.debugInfo();
    
    os << "\n Localhost Time:";
    os << local_host_time.debugInfo();
    
    os << "\n Trigger Board Clock Time:";
    os << trigger_board_time.debugInfo();

    return os.str();
}
