#ifndef _UBOONETYPES_LASERDATA_H
#define _UBOONETYPES_LASERDATA_H

#include "constants.h"
#include "boostSerialization.h"
#include "ub_LocalHostTime.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

/***
 *  Laser data struct
 ***/


class ub_LaserData final{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=7)
	ar 
	  & laserdata.laserid & laserdata.status
	  & laserdata.pos_rot & laserdata.pos_lin & laserdata.pos_att & laserdata.pos_iris
	  & laserdata.time_sec & laserdata.time_usec
	  & laserdata.count_trigger & laserdata.count_run & laserdata.count_laser
	  & laserdata.pos_tomg_1_axis1 & laserdata.pos_tomg_1_axis2
	  & laserdata.pos_tomg_2_axis1 & laserdata.pos_tomg_2_axis2;

      else if(version>0)
	std::cout << "\t\tVERSION HAS NO LASER DATA FORMAT." << std::endl;
    }
    
public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;

    ub_LaserData();

    void setData(char* const dataptr) noexcept;

    void setID(int const& i) noexcept;
    void setStatus(float const& stat) noexcept;
    void setPositionRotary(float const& pos) noexcept;
    void setPositionLinear(float const& pos) noexcept;
    void setPositionAttenuator(float const& pos) noexcept;
    void setPositionIris(float const& pos) noexcept;
    void setTime(float const& s, float const& us) noexcept;
    void setTimeSec(float const& s) noexcept;
    void setTimeUSec(float const& us) noexcept;
    void setCountTrigger(float const& count) noexcept;
    void setCountRun(float const& count) noexcept;
    void setCountLaser(float const& count) noexcept;
    void setTOMGBoxAxis1(float const& pos) noexcept;
    void setTOMGBoxAxis2(float const& pos) noexcept;
    void setTOMGFlangeAxis1(float const& pos) noexcept;
    void setTOMGFlangeAxis2(float const& pos) noexcept;

    int getID() const noexcept;
    float getStatus() const noexcept;
    float getPositionRotary() const noexcept;
    float getPositionLinear() const noexcept;
    float getPositionAttenuator() const noexcept;
    float getPositionIris() const noexcept;
    float getTimeSec() const noexcept;
    float getTimeUSec() const noexcept;
    float getCountTrigger() const noexcept;
    float getCountRun() const noexcept;
    float getCountLaser() const noexcept;
    float getTOMGBoxAxis1() const noexcept;
    float getTOMGBoxAxis2() const noexcept;
    float getTOMGFlangeAxis1() const noexcept;
    float getTOMGFlangeAxis2() const noexcept;

    std::string debugInfo()const noexcept;

private:

    typedef struct ub_LaserData_t{
      int laserid;            //which laser system: 1 or 2
      float status;           //Container for ervice messages
      float pos_rot;          //Position Rotary Heidenhain Encoder 
      float pos_lin;          //Position Linear Heidenhain Encoder 
      float pos_att;          //Position Attenuator Watt Pilot 
      float pos_iris;         //Position Iris Standa 
      float time_sec;         //Epoche System Time of Laser Server
      float time_usec;        //Fraction to be added to the epoche time in us
      float count_trigger;    //Trigger Counter by Heidenhain Encoder
      float count_run;        //Run Counter of step in calibration run 
      float count_laser;      //Number of pulses shot with UV laser
      float pos_tomg_1_axis1; //Motorized Mirror Zaber T-OMG at box, axis 1
      float pos_tomg_1_axis2; //Motorized Mirror Zaber T-OMG at box, axis 2
      float pos_tomg_2_axis1; //Motorized Mirror Zaber T-OMG at flange, axis 1
      float pos_tomg_2_axis2; //Motorized Mirror Zaber T-OMG at flange, axis 2
    } ub_LaserData_t;

    ub_LaserData_t laserdata;
};

}  // end of namespace sebs
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_LaserData, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)

#endif /* #ifndef BOONETYPES_H */



