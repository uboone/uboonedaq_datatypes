#ifndef _UBOONETYPES_CHANNELDATA_H
#define _UBOONETYPES_CHANNELDATA_H
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <inttypes.h>
#include "evttypes.h"

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/binary_object.hpp>

#include "constants.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;
 
/***
 *  Note: this is the serialization class that handles the card data.
 ***/

class ub_ChannelData {

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;

  ub_ChannelData(ub_VersionWord_t const version,
		 ub_RawData_t const rd):
  _version(version), _rawChannelData(rd){}

  //accessors for the raw data
  ub_VersionWord_t const& getRawDataVersionWord() { return _version; }

  //accessors for the channel-level data
  uint16_t const& getChannelHeaderWord() 
  { return CreateMarkedRawChannelData(); _markedRawChannelData()->getChannelHeaderWord(); }
  uint16_t const& getChannelTrailerWord()
  { return CreateMarkedRawChannelData(); _markedRawChannelData()->getChannelTrailerWord(); }

  uint16_t getChannelNumber() const 
  { return CreateMarkedRawChannelData(); _markedRawChannelDat()->getChannelNumber(); }

  std::vector<ub_RawDataWord_t> const& getDataVector();
  std::vector<ub_RawDataWord_t> const& { CreateUncompressedVector(); return _uncompressedVector; }

 private:

  const ub_VersionWord_t      _version;
  const ub_RawData_t          _rawChannelData;

  std::unique_ptr<ub_MarkedRawChannelData> _markedRawChannelData;
  void CreateMarkedRawChannelData();

  std::vector<ub_RawDataWord_t> _uncompressedVector;
  void CreateUncompressedVector();

  friend class boost::serialization::access;
  
  /***
      Use different save and load techniques here so that on the load, 
      we first read the data size, and then we declare space large 
      enough to hold it. After that we copy the data into our buffer, 
      and then swap the pointer to that buffer with out channelData member.
   ***/

  template<class Archive> 
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=VERSION::v6_00_00){
	ar & _version
	   & _rawChannelData
 	   & _markedRawChannelData
	   & _uncompressedVector;	
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
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::channelData, gov::fnal::uboone::datatypes::constants::VERSION)    

#endif /* #ifndef BOONETYPES_H */



