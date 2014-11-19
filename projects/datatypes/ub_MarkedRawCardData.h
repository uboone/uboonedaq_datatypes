#ifndef _UBOONE_TYPES_MARKEDRAWCARDDATA_H
#define _UBOONE_TYPES_MARKEDRAWCARDDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_MarkedRawChannelData.h"
#include "ub_ChannelDataCreatorHelperClass.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename CHANN, typename HEADER,typename TRAILER> 
class ub_MarkedRawCardData : public ub_MarkedRawDataBlock<HEADER,TRAILER>{
public: 
  typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::header_type card_header_type;
  typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer_type card_trailer_type;
  typedef CHANN card_channel_type;  
 
  static constexpr size_t size_of_data_overhead(){return size_of<card_header_type>() + size_of<card_trailer_type>();}
  
  template <typename MRCD> using dissector_type = ub_ChannelDataCreatorHelperClass<MRCD>;
 
  explicit ub_MarkedRawCardData(ub_RawData const rawdata): 
  ub_MarkedRawDataBlock<HEADER,TRAILER>(rawdata),
  _markedRawChannelsData{},_isValid{isValid()},_isFullyDissected{canFullyDissect()}{}
  //_markedRawChannelsData{},_isValid{isValid()},_isFullyDissected{false}{}

  uint32_t const& getCardIDAndModuleWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().id_and_module; }
  uint32_t const& getCardWordCountWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().word_count; }
  uint32_t const& getCardEventWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().event_number; }
  uint32_t const& getCardFrameWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().frame_number; }
  uint32_t const& getCardChecksumWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().checksum; }
  uint32_t const& getCardTrigFrameAndSampleWord() const { return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().trig_frame_and_sample; }
  
  std::vector<CHANN>  const&  getChannels();
  std::vector<CHANN>  const&  getChannels()const{return _markedRawChannelsData;};

  ub_MarkedRawCardData() = delete;
  ub_MarkedRawCardData& operator=(ub_MarkedRawCardData const &) = delete;
  
  void decompressChannels();
  void dissectChannels();  
  std::string debugInfo()const;

private:
	bool isValid();
	bool canFullyDissect();	
private:
	std::vector<CHANN>  _markedRawChannelsData;
	bool _isValid;
	bool _isFullyDissected;
};

template <typename CHANN, typename HEADER,typename TRAILER>
std::vector<CHANN>  const&  ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::getChannels() 
{ 
	if(!_isFullyDissected)
		dissectChannels();
		
	  return _markedRawChannelsData;
}

template <typename CHANN, typename HEADER,typename TRAILER>
void ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::dissectChannels()
{
	try
	{
		dissector_type<CHANN> dissector(ub_MarkedRawDataBlock<HEADER,TRAILER>::data());
		dissector.populateChannelDataVector(_markedRawChannelsData);
		_isFullyDissected=true;
	}
	catch(std::exception &ex)
	{
		throw;
	}
	catch(...)
	{
		throw std::runtime_error("Caught unknown exception in ub_MarkedRawCardData_v6::dissectChannels()."); 
	}
}

template <typename CHANN, typename HEADER,typename TRAILER>	  
bool ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::canFullyDissect()
{
	try
	{
		dissectChannels();
	}
	catch(std::exception &ex)
	{
		std::cout << "Exception:" << ex.what() << std::endl;
		return false;
	}
	return true;
}

template <typename CHANN, typename HEADER,typename TRAILER>	  
std::string ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::debugInfo()const
{
	std::ostringstream os;
	
	os << "Object " << demangle(typeid(this)) << "."<< std::endl;
	os << ub_MarkedRawDataBlock<HEADER,TRAILER>::header().debugInfo();
	os << ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer().debugInfo();
	
	os << " *Found " << std::dec << getChannels().size() << " channels." << std::endl;
	for(auto chan : getChannels())
		os << chan.debugInfo();
		
	// os <<  ub_MarkedRawDataBlock::debugInfo();
	return os.str();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 

#endif //_UBOONE_TYPES_MARKEDRAWCARDDATA_H
