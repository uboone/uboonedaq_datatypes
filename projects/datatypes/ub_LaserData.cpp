#include "ub_LaserData.h"
#include "uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

ub_LaserData::ub_LaserData() 
  :laserdata( {-1,-999,
	-999.999,-999.999,-999.999,-999.999,
	-999.999,-999.999,
	-1.0,-1.0,-1.0,
	-999.999,-999.999,-999.999,-999.999} )
{
}

void ub_LaserData::setData(char* const dataptr) noexcept{
  std::memcpy(&laserdata,dataptr,sizeof(ub_LaserData_t));
}
void ub_LaserData::setID(int const& i) noexcept{
  laserdata.laserid = i;
}
void ub_LaserData::setStatus(float const& stat) noexcept{
  laserdata.status = stat;
}
void ub_LaserData::setPositionRotary(float const& pos) noexcept{
  laserdata.pos_rot = pos;
}
void ub_LaserData::setPositionLinear(float const& pos) noexcept{
  laserdata.pos_lin = pos;
}
void ub_LaserData::setPositionAttenuator(float const& pos) noexcept{
  laserdata.pos_att = pos;
}
void ub_LaserData::setPositionIris(float const& pos) noexcept{
  laserdata.pos_iris = pos;
}
void ub_LaserData::setTime(float const& s, float const& us) noexcept{
  setTimeSec(s);
  setTimeUSec(us);
}
void ub_LaserData::setTimeSec(float const& s) noexcept{
  laserdata.time_sec = s;
}
void ub_LaserData::setTimeUSec(float const& us) noexcept{
  laserdata.time_usec = us;
}
void ub_LaserData::setCountTrigger(float const& count) noexcept{
  laserdata.count_trigger = count;
}
void ub_LaserData::setCountRun(float const& count) noexcept{
  laserdata.count_run = count;
}
void ub_LaserData::setCountLaser(float const& count) noexcept{
  laserdata.count_laser = count;
}
void ub_LaserData::setTOMGBoxAxis1(float const& pos) noexcept{
  laserdata.pos_tomg_1_axis1 = pos;
}
void ub_LaserData::setTOMGBoxAxis2(float const& pos) noexcept{
  laserdata.pos_tomg_1_axis2 = pos;
}
void ub_LaserData::setTOMGFlangeAxis1(float const& pos) noexcept{
  laserdata.pos_tomg_2_axis1 = pos;
}
void ub_LaserData::setTOMGFlangeAxis2(float const& pos) noexcept{
  laserdata.pos_tomg_2_axis2 = pos;
}

int ub_LaserData::getID() const noexcept{
  return laserdata.laserid;
}
float ub_LaserData::getStatus() const noexcept{
  return laserdata.status;
}
float ub_LaserData::getPositionRotary() const noexcept{
  return laserdata.pos_rot;
}
float ub_LaserData::getPositionLinear() const noexcept{
  return laserdata.pos_lin;
}
float ub_LaserData::getPositionAttenuator() const noexcept{
  return laserdata.pos_att;
}
float ub_LaserData::getPositionIris() const noexcept{
  return laserdata.pos_iris;
}
float ub_LaserData::getTimeSec() const noexcept{
  return laserdata.time_sec;
}
float ub_LaserData::getTimeUSec() const noexcept{
  return laserdata.time_usec;
}
float ub_LaserData::getCountTrigger() const noexcept{
  return laserdata.count_trigger;
}
float ub_LaserData::getCountRun() const noexcept{
  return laserdata.count_run;
}
float ub_LaserData::getCountLaser() const noexcept{
  return laserdata.count_laser;
}
float ub_LaserData::getTOMGBoxAxis1() const noexcept{
  return laserdata.pos_tomg_1_axis1;
}
float ub_LaserData::getTOMGBoxAxis2() const noexcept{
  return laserdata.pos_tomg_1_axis2;
}
float ub_LaserData::getTOMGFlangeAxis1() const noexcept{
  return laserdata.pos_tomg_2_axis1;
}
float ub_LaserData::getTOMGFlangeAxis2() const noexcept{
  return laserdata.pos_tomg_2_axis2;
}

std::string ub_LaserData::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n\t Laser system id: " << laserdata.laserid;
    os << "\n\t Laser status code: " << laserdata.status;
    os << "\n\t Position Rotary Heidenhain Encoder: " << laserdata.pos_rot;
    os << "\n\t Position Linear Heidenhain Encoder: " << laserdata.pos_lin;
    os << "\n\t Position Attenuator Watt Pilot: " << laserdata.pos_att;
    os << "\n\t Position Iris Standa: " << laserdata.pos_iris;
    os << "\n\t Epoche system time of server (s,us): (" 
       << laserdata.time_sec << "," << laserdata.time_usec << ")";
    os << "\n\t Trigger Counter by Heidenhain Encoder: " << laserdata.count_trigger;
    os << "\n\t Run counter of step in calibration runs: " << laserdata.count_run;
    os << "\n\t Number of pulses shot with UV laser: " << laserdata.count_laser;
    os << "\n\t Motorized Mirror Zaber T-OMG at box, axis 1: " << laserdata.pos_tomg_1_axis1;
    os << "\n\t Motorized Mirror Zaber T-OMG at box, axis 2: " << laserdata.pos_tomg_1_axis2;
    os << "\n\t Motorized Mirror Zaber T-OMG at flange, axis 1: " << laserdata.pos_tomg_2_axis1;
    os << "\n\t Motorized Mirror Zaber T-OMG at flange, axis 2: " << laserdata.pos_tomg_2_axis2;

    return os.str();
}
