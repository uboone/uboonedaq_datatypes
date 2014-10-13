#ifndef _UBOONETYPES_SEBFRAGMENT_H
#define _UBOONETYPES_SEBFRAGMETN_H
#include <memory>
#include <map>
#include <algorithm>
#include <sys/types.h>
#include <stdexcept>
#include <inttypes.h>
#include "evttypes.h"

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/binary_object.hpp>

#include "constants.h"
#include "ub_CrateData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;
 

 struct typedef ub_sebHeader{
   bool               complete;
   ub_SEBFragmentType fragment_type;
   uint32_t           size;
   uint8_t            fragment_number;
   uint32_t           event_number;
   uint32_t           frame_number;
   uint32_t           seb_time_sec;
   uint32_t           seb_time_usec;
   ub_sebHeader( bool c, ub_SEBFragmentType ft, uint32_t s, uint8_t fn,
		 uint32_t ev, uint32_t fr, uint32_t sec, uint32_t usec):
   complete(c), fragment_type (ft), size(s), fragment_number(fn),
     event_number(ev), frame_number(fr), seb_time_sec(sec), seb_time_usec(usec) {}
 } ub_sebHeader_t;


/***
 *  Note: this is the serialization class that handles the data.
 ***/
class ub_sebFragment {

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION::v6_00_00;

  ub_sebFragment( ub_sebHeader_t const& h, std::vector<ub_RawDataWord_t> const& d, ub_VersionWord_t const& v)
    { setSEBHeader(h); _raw_version = v; _data = d; _crateData = ubCrateData(v,d); }

  ub_sebFragment( ub_sebHeader_t const& h, std::vector<ub_RawDataWord_t> const& d, ub_CrateData const& cd)
    { setSEBHeader(h); _raw_version = cd.getRawDataVersionWord(); _data = d; _crateData = cd; }

  ub_VersionWord_t const& getRawDataVersionWord() { return _raw_version; }

  ub_Base_CrateData const& getCrateData() { return _crateData; }
  void setCrateData(ub_CrateData const& cd) { _crateData = cd; _raw_version = cd.getRawDataVersionWord(); }

  bool const& isComplete() { return _seb_header.complete; }
  ub_FragmentType const& getSEBFragmentType() { return _seb_header.fragment_type; }
  uint32_t const& getSize() { return _seb_header.size; }
  uint8_t  const& getFragmentNumber() { return _seb_header.fragment_number; }
  uint32_t cosnt& getEventNumber() { return _seb_header.event_number; }
  uint32_t cosnt& getFrameNumber() { return _seb_header.frame_number; }
  uint32_t cosnt& getTimeSec() { return _seb_header.seb_time_sec; }
  uint32_t cosnt& getTimeUSec() { return _seb_header.seb_time_usec; }

  ub_sebHeader_t cont& getSEBHeader() { return _seb_header; }
  void setSEBHeader(ub_sebHeader_t const& header) { _seb_header = header; }
  void setSEBHeader( bool c, ub_SEBFragmentType ft, uint32_t s, uint8_t fn,
		     uint32_t ev, uint32_t fr, uint32_t sec, uint32_t usec)
  {_seb_header = ub_sebHeader(c,ft,s,fn,ev,fr,sec,usec); }

 private:
  ub_sebHeader_t                _seb_header;
  ub_VersionWord_t              _raw_version;
  std::vector<ub_RawDataWord_t> _data;
  ub_CrateData                  _crateData;

  friend class boost::serialization::access;
  
  template<class Archive> 
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=VERSION::v6_00_00){
	ar & _seb_header.complete
	   & _seb_header.size
	   & _seb_header.fragment_number
	   & _seb_header.event_number
	   & _seb_header.frame_number
	   & _seb_header.seb_time_sec
	   & _seb_header.seb_time_usec
	   & _raw_version
	   & _data
	   & _crateData;
      }
      else if(version<VERSION::v6_00_00) {
	std::cout << "==========================================================================" << std::endl;
	std::cout << "ERROR: YOU ARE TRYING TO WRITE DATA INCOMPATIBLE WITH NEW VERSIONS." << std::endl;
	std::cout << "       version read in = " << version << std::endl;
	std::cout << "       YOU SHOULD USE AN OLDER VERSION OF UBOONEDAQ_DATATYPES TO READ." << std::endl;
	std::cout << "==========================================================================" << std::endl;
	throw std::runtime_error("Incompatible data.");
      }
      else{
	throw std::runtime_error("Invalid version number.");
      }
      
    }//end serialize
  
};
 
 
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_CrateData, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif /* #ifndef BOONETYPES_H */



