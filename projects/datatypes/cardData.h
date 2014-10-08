#ifndef _UBOONETYPES_CARDDATA_H
#define _UBOONETYPES_CARDDATA_H
#include <memory>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream> // For debugging
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

class cardData {

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION::v6_00_00;
  
  cardData(){}

  //accessors for the raw data
  ub_VersionWord_t const& getRawDataVersionWord() { return _version; }
  
  //accesors for the card-level data
  uint32_t const& getCardIDAndModuleWord();
  uint32_t const& getCardWordCountWord();
  uint32_t const& getCardEventWord();
  uint32_t const& getCardFrameWord();
  uint32_t const& getCardChecksumWord();
  uint32_t const& getCardTrigFrameAndSampleWord();
  uint32_t getID() const;
  uint32_t getModule() const;
  uint32_t getEvent() const;
  uint32_t getFrame() const;
  uint32_t getChecksum() const;
  uint32_t getWordCount() const;
  uint32_t getTrigFrame() const;
  uint8_t  getTrigFrameMod16() const;
  uint32_t getTrigSample() const;
  std::vector<ub_RawDataWord_t> const& getDataVector();

  //accessors for the channel-level data
  size_t getNChannels() const { FillChannelDataVector(); return _channelDataVector.size(); }
  std::vector<channelData> const& getChannelDataVector() { FillChannelDataVector(); return _channelDataVector; }
  channelData const& getChannelData(unsigned int i) { FillChannelDataVector(); return _channelDataVector.at(i); }

  uint16_t const& getChannelHeaderWord(unsigned int i)
  { return getChannelData(i).getChannelHeaderWord(); }
  uint16_t const& getChannelTrailerWord(unsigned int i)
  { return getChannelData(i).getChannelTrailerWord(); }

  uint16_t getChannelNumber(unsigned int i) const { return getChannelData(j).getChannelNumber(); }

  std::vector<ub_RawDataWord_t> const& getChannelDataVector(unsigned int i);
  { return getChannelData(i).getDataVector(); }
  std::vector<ub_RawDataWord_t> const& getUncompressedChannelDataVector(unsigned int i);
  { return getChannelData(i).getUncompressedDataVector(); }


 private:
  //this is the raw data coming from the card
  const ub_VersionWord_t      _version;
  const ub_RawData_t          _rawCardData;
  std::unique_ptr<ub_MarkedRawCardData> _markedRawCardData;

  void CreateMarkedRawCardData();

  std::vector<channelData> _channelDataVector;
  void FillChannelDataVector();

  friend class boost::serialization::access;
  
  template<class Archive> 
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version==VERSION::v6_00_00){
	ar & data_vector
	   & channel_locations_vector;
      }
      else if(version<VERSION::v6_00_00) {
	std::cout << "==========================================================================" << std::endl;
	std::cout << "ERROR: YOU ARE READING TRYING TO READ DATA INCOMPATIBLE WITH NEW VERSIONS." << std::endl;
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
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::cardData, gov::fnal::uboone::datatypes::constants::VERSION::v6_00_00)    

#endif /* #ifndef BOONETYPES_H */



