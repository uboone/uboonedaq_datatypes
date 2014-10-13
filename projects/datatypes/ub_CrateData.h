#ifndef _UBOONETYPES_CRATEDATA_H
#define _UBOONETYPES_CRATEDATA_H
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
#include "ub_MarkedRawCrateData.h"
#include "ub_CardData.h"
#include "ub_ChannelData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;
 
/***
 *  Note: this is the serialization class that handles the data.
 ***/
 
class ub_CrateData{

 public:
  static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION::v6_00_00;

 ub_CrateData(ub_VersionWord_t const version, 
	      ub_RawData_t const rd ):
  _version(version), _rawCrateData(rd){}
  
  //accessors for the raw data
  ub_VersionWord_t const& getRawDataVersionWord() { return _version; }
  
  //accesors for the crate-level data
  uint32_t const& getHeaderWord() { CreateMarkedRawCrateData(); return _markedRawCrateData->getHeaderWord(); }
  uint32_t const& getTrailerWord() { CreateMarkedRawCrateData(); return _markedRawCrateData->getTrailerWord(); }
  std::vector<ub_RawDataWord_t> const& getDataVector();
  
 //accessors for the card-level data
  size_t getNCards() const { FillCardDataVector(); return _cardDataVector.size(); }
  std::vector<ub_CardData> const& getCardDataVector() { FillCardDataVector(); return _cardDataVector; }
  ub_CardData const& getCardData(unsigned int i) { FillCardDataVector(); return _cardDataVector.at(i); }

  uint32_t const& getCardIDAndModuleWord(unsigned int i) { return getCardData(i).getCardIDAndModuleWord(); }
  uint32_t const& getCardWordCountWord(unsigned int i) { return getCardData(i).getCardWordCountWord(); }
  uint32_t const& getCardEventWord(unsigned int i) { return getCardData(i).getCardEventWord(); }
  uint32_t const& getCardFrameWord(unsigned int i) { return getCardData(i).getCardFrameWord(); }
  uint32_t const& getCardChecksumWord(unsigned int i) { return getCardData(i).getCardChecksumWord(); }
  uint32_t const& getCardTrigFrameAndSampleWord(unsigned int i) { return getCardData(i).getCardTrigFrameAndSampleWord(); }
  uint32_t getID(unsigned int i) const { return getCardData(i).getID(); }
  uint32_t getModule(unsigned int i) const {return getCardData(i).getModule(); }
  uint32_t getEvent(unsigned int i) const {return getCardData(i).getEvent(); }
  uint32_t getFrame(unsigned int i) const {return getCardData(i).getFrame(); }
  uint32_t getChecksum(unsigned int i) const {return getCardData(i).getChecksum(); }
  uint32_t getWordCount(unsigned int i) const {return getCardData(i).getWordCount(); }
  uint32_t getTrigFrame(unsigned int i) const {return getCardData(i).getTrigFrame(); }
  uint8_t  getTrigFrameMod16(unsigned int i) const {return getCardData(i).getTrigFrameMod16(); }
  uint32_t getTrigSample(unsigned int i) const {return getCardData(i).getTrigSample(); }
  std::vector<ub_RawDataWord_t> const& getCardDataVector(unsigned int i) {return getCardData(i).getDataVector(); }
  
  //accessors for the channel-level data
  size_t getNChannels(unsigned int) const { return getCardData(i).getChannelDataVector().size(); }
  std::vector<ub_ChannelData> const& getChannelDataVector() { return getCardData(i).getChannelDataVector(); }
  ub_ChannelData const& getChannelData(unsigned int i_card, unsigned int i_ch) 
  { return  getCardData(i_card).getChannelDataVector().at(i_ch); }

  uint16_t const& getChannelHeaderWord(unsigned int i,unsigned int j)
  { return getCardData(i).getChannelData(j).getChannelHeaderWord(); }
  uint16_t const& getChannelTrailerWord(unsigned int i,unsigned int j)
  { return getCardData(i).getChannelData(j).getChannelTrailerWord(); }

  uint16_t getChannelNumber(unsigned int i, unsigned int j) const 
  { return getCardData(i).getChannelData(j).getChannelNumber(); }

  std::vector<ub_RawDataWord_t> const& getChannelDataVector(unsigned int i, unsigned int j);
  { return getCardData(i).getChannelData(j).getDataVector(); }
  std::vector<ub_RawDataWord_t> const& getUncompressedChannelDataVector(unsigned int i, unsigned int j);
  { return getCardData(i).getChannelData(j).getUncompressedDataVector(); }
  
  
 private:
  
  //this is the raw data coming from the crate
  const ub_VersionWord_t      _version;
  const ub_RawData_t          _rawCrateData;

  std::unique_ptr<ub_MarkedRawCrateData> _markedRawCrateData;
  void CreateMarkedRawCrateData();

  std::vector<ub_CardData> _cardDataVector;
  void FillCardDataVector();

  friend class boost::serialization::access;
  
  template<class Archive> 
    void serialize(Archive & ar, const unsigned int version)
    {
      if(version>=VERSION::v6_00_00){
	ar & _version
	   & _rawCrateData
 	   & _markedRawCrateData
	   & _cardDataVector;
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



