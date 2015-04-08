#include "ub_BeamData.h"

using namespace gov::fnal::uboone::datatypes;

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamData &bd) {
    os << std::left
       << std::setw(12) << bd.getDeviceName()
       << std::setw(10) << bd.getSeconds() << "."
       << std::right << std::setw(3)  << std::setfill('0') << bd.getMilliSeconds()
       << std::setfill(' ') << "  "
       << std::setw(14) << bd.getData().at(0);

    for (size_t i=1; i<bd.getData().size(); i++)
        os <<", "<<bd.getData()[i];

    os << std::setw(6) << bd.getUnits()<<std::endl;

    return os;
}

ub_BeamData::ub_BeamData():device_name {""},units {""},seconds {0},milli_seconds {0} {
}


std::string ub_BeamData::getDeviceName() const noexcept{
    return device_name;
}

std::string ub_BeamData::getUnits() const noexcept{
    return units;
}
uint32_t ub_BeamData::getSeconds() const noexcept{
    return seconds;
} // GPS clock. Since Jan 1, 2012.

uint16_t ub_BeamData::getMilliSeconds() const noexcept{
    return milli_seconds;
}

std::vector<double> const& ub_BeamData::getData() const noexcept{
    return device_data;
}

void ub_BeamData::setDeviceName(std::string const& val) noexcept{
    device_name=val;
}

void ub_BeamData::setUnits(std::string const& val) noexcept{
    units=val;
}

void ub_BeamData::setSeconds(uint32_t const& val) noexcept{
    seconds=val;
}

void ub_BeamData::setMilliSeconds(uint16_t const& val) noexcept{
    milli_seconds=val;
}

void ub_BeamData::setData(std::vector<double> const& val) noexcept{
    device_data=val;
}

void ub_BeamData::pushData(double const& val) noexcept{
    device_data.push_back(val);
}
