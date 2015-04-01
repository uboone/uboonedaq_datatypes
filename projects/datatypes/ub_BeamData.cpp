#include "ub_BeamData.h"

using namespace gov::fnal::uboone::datatypes;

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamData &bd)
{
  os << std::left
     << std::setw(12) << bd.getDeviceName()
     << std::setw(10) << bd.getSeconds() << "."
     << std::right << std::setw(3)  << std::setfill('0') << bd.getMilliSeconds()
     << std::setfill(' ') << "  "
     << std::setw(14) << bd.getData().at(0);
  for (size_t i=1;i<bd.getData().size();i++) os <<", "<<bd.getData()[i];
  os << std::setw(6) << bd.getUnits()<<std::endl;

  return os;
}

ub_BeamData::ub_BeamData()
{
  device_name="";
  units="";
  seconds=0;
  milli_seconds=0;
  device_data.resize(0);
}