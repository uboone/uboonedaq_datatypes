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
   typedef std::map<int,std::unique_ptr<tpc_crate_data_t>> tpc_seb_map_t;
   typedef std::map<int,std::unique_ptr<pmt_crate_data_t>> pmt_seb_map_t;

  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
  ub_EventRecord():_global_header(),_tpc_seb_map(),_pmt_seb_map(){}  
  void setGlobalHeader (global_header_t & header) { _global_header = header; }
  global_header_t& getGlobalHeader() { return _global_header; }

  void insertSEB(std::unique_ptr<tpc_crate_data_t>& crate) {_tpc_seb_map.emplace(crate->crateHeader()->crate_number, std::move(crate));}
  void insertSEB(std::unique_ptr<pmt_crate_data_t>& crate) {_pmt_seb_map.emplace(crate->crateHeader()->crate_number, std::move(crate));}
  
  const tpc_seb_map_t& getTPCSEBMap() const { return _tpc_seb_map; }
  const pmt_seb_map_t& getPMTSEBMap() const { return _pmt_seb_map; }
  tpc_seb_map_t& getTPCSEBMap() { return _tpc_seb_map; }
  pmt_seb_map_t& getPMTSEBMap() { return _pmt_seb_map; }

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
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=3)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   & _tpc_seb_map
	   & _pmt_seb_map
	   ;

      else if(version>1)
	ar //& er_IO_mode
	   & _global_header
	   //& trigger_data
	   //& gps_data
	   //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
	   & _tpc_seb_map
	   & _pmt_seb_map
	   ;
      else if(version>0)
	ar //& er_IO_mode
	   & _global_header
	   & _tpc_seb_map
	   & _pmt_seb_map
	   ;
    }
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_EventRecord, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif /* #ifndef BOONETYPES_H */
