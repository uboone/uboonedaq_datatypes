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
#include "ub_MarkedRawCardData.h"
#include "ub_ChannelData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;
 
/***
 *  Note: this is the serialization class that handles the card data.
 ***/

class ub_CardData {

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION::v6_00_00;
  
  ub_CardData(ub_VersionWord_t const version, 
	   ub_RawData_t const rd ):
  _version(version), _rawCrateData(rd){}
  
  //accessors for the raw data
  ub_VersionWord_t const& getRawDataVersionWord() { return _version; }
  
  //accesors for the card-level data
  uint32_t const& getCardIDAndModuleWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardIDAndModuleWord(); }
  uint32_t const& getCardWordCountWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardWordCountWord(); }
  uint32_t const& getCardEventWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardEventWord(); }
  uint32_t const& getCardFrameWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardFrameWord(); }
  uint32_t const& getCardChecksumWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardChecksumWord(); }
  uint32_t const& getCardTrigFrameAndSampleWord() { CreateMarkedRawCardData(); return _markedRawCardData->getCardTrigFrameAndSampleWord(); }
  uint32_t getID() const { CreateMarkedRawCardData(); return _markedRawCardData->getID(); }
  uint32_t getModule() const { CreateMarkedRawCardData(); return _markedRawCardData->getModule(); }
  uint32_t getEvent() const { CreateMarkedRawCardData(); return _markedRawCardData->getEvent(); }
  uint32_t getFrame() const { CreateMarkedRawCardData(); return _markedRawCardData->getFrame(); }
  uint32_t getChecksum() const { CreateMarkedRawCardData(); return _markedRawCardData->getChecksum(); }
  uint32_t getWordCount() const { CreateMarkedRawCardData(); return _markedRawCardData->getWordCount(); }
  uint32_t getTrigFrame() const { CreateMarkedRawCardData(); return _markedRawCardData->getTrigFrame(); }
  uint8_t  getTrigFrameMod16() const { CreateMarkedRawCardData(); return _markedRawCardData->getTrigFrameMod16(); }
  uint32_t getTrigSample() const { CreateMarkedRawCardData(); return _markedRawCardData->getTrigSample(); }
  std::vector<ub_RawDataWord_t> const& getDataVector();

  //accessors for the channel-level data
  size_t getNChannels() const { FillChannelDataVector(); return _channelDataVector.size(); }
  std::vector<ub_ChannelData> const& getChannelDataVector() { FillChannelDataVector(); return _channelDataVector; }
  ub_ChannelData const& getChannelData(unsigned int i) { FillChannelDataVector(); return _channelDataVector.at(i); }

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

  std::vector<ub_ChannelData> _channelDataVector;
  void FillChannelDataVector();

  friend class boost::serialization::access;
  
  template<class Archive> 
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version==VERSION::v6_00_00){
	ar & _version
	   & _rawCrateData
 	   & _markedRawCardData
	   & _channelDataVector;
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



