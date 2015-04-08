#include "ub_GPS_DataTypes.h"

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
