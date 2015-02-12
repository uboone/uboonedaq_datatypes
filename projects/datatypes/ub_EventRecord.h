#ifndef _UBOONETYPES_EVENTRECORD_H
#define _UBOONETYPES_EVENTRECORD_H


#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"
#include "raw_data_access.h"


/***
    The ub_EventRecord is meant to house all of the components of the 
    final data format as it leaves the assembler and is written to 
    disk. The data will be written as a boost binary_archive, so 
    we may version different pieces accordingly. Along with all the
    (independently) serialized headers, we have a map that pairs 
    crate headers and crate data.
 ***/


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone::datatypes;

class ub_EventRecord {

 public:
   typedef std::tuple<raw_data_containter<raw_data_type>, 
		      std::unique_ptr<ub_RawData>,
		      std::unique_ptr<tpc_crate_data_t>> tpc_crate_data_tuple_t;
   typedef std::map<int,tpc_crate_data_tuple_t> tpc_seb_map_t;
   
   typedef std::map<int,tpc_crate_data_t const&> tpc_map_t;
   
   typedef std::tuple<raw_data_containter<raw_data_type>,
		      std::unique_ptr<ub_RawData>,
		      std::unique_ptr<pmt_crate_data_t>> pmt_crate_data_tuple_t;
   typedef std::map<int,pmt_crate_data_tuple_t> pmt_seb_map_t;
   
   typedef std::map<int,pmt_crate_data_t const&> pmt_map_t;
   
   typedef std::vector<raw_data_containter<raw_data_type>const*> fragment_references_t;
   
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
  ub_EventRecord():_global_header(),_tpc_seb_map(),_pmt_seb_map(){}  
  void setGlobalHeader (global_header_t & header) { _global_header = header; }
  global_header_t& getGlobalHeader() { return _global_header; }

  void addFragment(raw_data_containter<raw_data_type>& fragment);
  
  const tpc_map_t getTPCSEBMap() const;
  const pmt_map_t getPMTSEBMap() const;
  void  getFragments(fragment_references_t& fragments) const;

//  void setTriggerData (triggerData tD) { trigger_data = tD; }
//  void setGPS (ub_GPS g) { gps_data = g; }
//  void setBeamHeader (beamHeader bH) { beam_header = bH; }
//  void insertBeamData (beamData bD) { beam_data_vector.push_back(bD); }
//  ub_GlobalHeader getGlobalHeader() { return global_header; }
//  triggerData getTriggerData() { return trigger_data; }
//  ub_GPS getGPS() { return gps_data; }
//  beamHeader getBeamHeader() { return beam_header; }
//  std::vector<beamData> getBeamDataVector() { return beam_data_vector; }
//  triggerData* getTriggerDataPtr() { return &trigger_data; }
//  ub_GPS* getGPSPtr() { return &gps_data; }
//  beamHeader* getBeamHeaderPtr() { return &beam_header; }
//  int getBeamDataVecotr_size() { return beam_data_vector.size(); }
//  void clearBeamDataVector() { beam_data_vector.clear(); }
 private:
  global_header_t  _global_header;
  tpc_seb_map_t    _tpc_seb_map;
  pmt_seb_map_t    _pmt_seb_map;

//  triggerData trigger_data;
//  ub_GPS gps_data;
//  beamHeader beam_header;
//  std::vector<beamData> beam_data_vector;  
//  uint8_t er_IO_mode;

  friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {      
        if(version>=3)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   ;

      else if(version>1)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   ;
      else if(version>0)
	ar //& er_IO_mode
	   & _global_header
	   ;
    
      //this must be the last step
      fragment_references_t fragments;
      getFragments(fragments);      
      for(auto const& fragment : fragments)
      {
          ;
      }
    }
 
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      std::vector<raw_data_containter<raw_data_type>> fragments;
    
       if(version>=3)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   ;

      else if(version>1)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   ;
      else if(version>0)
	ar //& er_IO_mode
	   & _global_header
	   ;

    //this must be the last step	   
    for(auto& fragment:fragments)
	    addFragment(fragment);
	   
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_EventRecord, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif /* #ifndef BOONETYPES_H */
