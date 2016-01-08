#ifndef _UBOONETYPES_FEMBEAMTRIGGEROUTPUT_H
#define _UBOONETYPES_FEMBEAMTRIGGEROUTPUT_H

#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"
#include "uboone_data_utils.h"
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
  
  class ub_FEMBeamTriggerOutput final{
    
  private:
    friend class boost::serialization::access;
    
    template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
	if(version>=11)
	  ar & window_start
	    & window_end
	    & maxdiff
	    & maxhit
	    & fire_time;
	
	else if(version>0)
	  ar & window_start
	    & window_end
	    & maxdiff
	    & maxhit
	    & fire_time;
      }
    
  public:
    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;
    
    /// Default ctor
  ub_FEMBeamTriggerOutput() //change here, originally has nwindows as argument
    : window_start ( 0 ) //orignally has (nwindow,0) I don't think we need this, it seems it's just a fix number all the time
      , window_end    ( 0 ) //same as the start?
      , maxdiff       ( 0 )   
      , maxhit        ( 0 )     
      , fire_time     ( -1 )  
      {}
    
    /// Default dtor
    ~ub_FEMBeamTriggerOutput() {}
    
    size_t window_start; ///< Window start tick
    size_t window_end;   ///< Window end   tick
    short maxdiff;       ///< PHMAX sum vector
    short maxhit;        ///< Multiplicity sum vector
    int fire_time;    ///< Trigger decision times (-1 means not fired)
    
    std::string debugInfo() const noexcept;
    
  private:
    
  };
 
}  // end of namespace sebs
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_FEMBeamTriggerOutput, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)

#endif /* #ifndef BOONETYPES_H */



