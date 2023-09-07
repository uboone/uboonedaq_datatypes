#ifndef _UBOONETYPES_TRIGGERCOUNTER_H
#define _UBOONETYPES_TRIGGERCOUNTER_H

#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"
#include "ub_Trigger_DataTypes_v6.h"
#include <string>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

/***
 * Trigger counter keeps track of triggers from all events seen at
 * assembler, before prescaling.
 ***/

 enum ub_TriggerType {
   pmt_beam = 0,
   pmt_cosmic,
   pc,
   ext,
   active,
   gate2,
   gate1,
   veto,
   calib,
   gatefake,
   beamfake,
   spare1,
   all,
   ntriggers
 };

 typedef struct {
   uint32_t pmt_beam;
   uint32_t pmt_cosmic;
   uint32_t pc;
   uint32_t ext;
   uint32_t active;
   uint32_t gate2;
   uint32_t gate1;
   uint32_t veto;
   uint32_t calib;
   uint32_t gatefake;
   uint32_t beamfake;
   uint32_t spare1;
 } ub_TriggerSummary_t;

class ub_TriggerCounter final{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=10)
	ar & _n_total
	   & _tc.pmt_beam & _tc.pmt_cosmic
	   & _tc.pc & _tc.ext & _tc.active
	   & _tc.gate2 & _tc.gate1 
	   & _tc.veto & _tc.calib
	   & _tc.gatefake & _tc.beamfake
	   & _tc.spare1;
      else if(version>=9)
	ar & _n_total
	   & _tc.pc & _tc.ext & _tc.active
	   & _tc.gate2 & _tc.gate1 
	   & _tc.veto & _tc.calib
	   & _tc.gatefake & _tc.beamfake
	   & _tc.spare1;

      else if(version>0)
	ar & _n_total
	   & _tc.pc & _tc.ext & _tc.active
	   & _tc.gate2 & _tc.gate1 
	   & _tc.veto & _tc.calib
	   & _tc.gatefake & _tc.beamfake
	   & _tc.spare1;
    }
    
public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;
    ub_TriggerCounter();

    void reset();
    void increment( ub_Trigger_Data_v6 const& td, bool same_total=false);

    uint32_t get_N_Total()      const { return _n_total; }
    uint32_t get_N_PMT_Beam()   const { return _tc.pmt_beam; }
    uint32_t get_N_PMT_Cosmic() const { return _tc.pmt_cosmic; }
    uint32_t get_N_PC()         const { return _tc.pc; }
    uint32_t get_N_EXT()        const { return _tc.ext; }
    uint32_t get_N_Active()     const { return _tc.active; }
    uint32_t get_N_Gate2()      const { return _tc.gate2; }
    uint32_t get_N_Gate1()      const { return _tc.gate1; }
    uint32_t get_N_Veto()       const { return _tc.veto; }
    uint32_t get_N_Calib()      const { return _tc.calib; }
    uint32_t get_N_GateFake()   const { return _tc.gatefake; }
    uint32_t get_N_BeamFake()   const { return _tc.beamfake; }
    uint32_t get_N_Spare1()     const { return _tc.spare1; }

    std::string debugInfo()const noexcept;

    bool prescalePass( std::map< uint16_t, float > const&, uint16_t, double );

private:

    uint32_t _n_total;
    ub_TriggerSummary_t _tc;
    

};

}  // end of namespace sebs
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_TriggerCounter, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)

#endif /* #ifndef BOONETYPES_H */



