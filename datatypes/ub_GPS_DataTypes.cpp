#include "ub_GPS_DataTypes.h"
#include "uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

uint32_t ub_GPS::getLower() const noexcept {
    return lower;
}
uint32_t ub_GPS::getUpper() const noexcept {
    return upper;
}
void ub_GPS::setLower(uint32_t const& lower_) noexcept {
    lower = lower_;
}
void ub_GPS::setUpper(uint32_t const& upper_) noexcept {
    upper = upper_;
}

std::string ub_GPS::debugInfo()const noexcept {
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n GPS time (upper,lower) " << upper << ", " << lower;
    return os.str();
}

bool ub_GPS_Time::wasSet() const noexcept{
  return (second != 0 || micro !=0 || nano !=0);
}

ub_GPS_Time::ub_GPS_Time (uint32_t sec, uint32_t usec, uint32_t nano_sec)
:second(sec),micro(usec),nano(nano_sec){}

std::string ub_GPS_Time::debugInfo() const noexcept {
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n GPS time (second,micro,nano) " << second << ", " << micro << ", " << nano;
    return os.str();
}


void HasGPSTime::copyOut(ub_GPS_Time& target) noexcept  {
target=_myValue; 
}

void HasGPSTime::copyIn(ub_GPS_Time const& source) noexcept {
_myValue=source;
}  
