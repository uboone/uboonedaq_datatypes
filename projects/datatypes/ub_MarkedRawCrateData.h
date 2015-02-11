#ifndef _UBOONE_TYPES_MARKEDRAWCRATEDATA_H
#define _UBOONE_TYPES_MARKEDRAWCRATEDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_XMITEventHeaderTrailer_v0.h"
#include "ub_CardDataCreatorHelperClass.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
	
template <typename CARD> class ub_MarkedRawCrateData : 
public ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer>{
  public:
	template <typename MRCD> using dissector_type = ub_CardDataCreatorHelperClass<MRCD>;
    
	explicit ub_MarkedRawCrateData(ub_RawData const rawdata): 
	ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer>(rawdata),
	_markedRawCardsData{},_dissectableDataSize{0},_isValid{isValid()},
	_isFullyDissected{canFullyDissect()},_crateHeader{nullptr},_createHeaderFromData{false}{}

	explicit ub_MarkedRawCrateData(ub_RawData const rawdata,bool createHeaderFromData): 
	ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer>(rawdata),
	_markedRawCardsData{},_dissectableDataSize{0},_isValid{isValid()},
	_isFullyDissected{canFullyDissect()},_crateHeader{nullptr},_createHeaderFromData{createHeaderFromData}{}
	
       uint32_t const& getHeaderWord() const {return header().raw_data; } 
	uint32_t const& getTrailerWord() const {return trailer().raw_data; } 
	std::vector<CARD> const&  getCards() ;
	std::vector<CARD> const&  getCards() const {return _markedRawCardsData;}
		
	ub_MarkedRawCrateData() = delete;
	//ub_MarkedRawCrateData(ub_MarkedRawCrateData const &) = delete;
	ub_MarkedRawCrateData& operator=(ub_MarkedRawCrateData const &) = delete;
	size_t getSizeOfCardsData() const {return data().size();};
	size_t getSizeOfRAWCrateData() const {return rawdata().size();};
	
	void dissectCards();
	std::string debugInfo()const;
	
	size_t getSizeOfDissectableCrateData() const {
	  assert(_dissectableDataSize !=0 ); return _dissectableDataSize;};
	  
	std::unique_ptr<typename CARD::ub_CrateHeader>& crateHeader();
	std::unique_ptr<typename CARD::ub_CrateHeader> const& crateHeader()const{return _crateHeader;}; 
private:
	bool isValid();
	bool canFullyDissect();	
private:
	std::vector<CARD> _markedRawCardsData;	
	size_t _dissectableDataSize; 
	bool _isValid;
	bool _isFullyDissected;	
	std::unique_ptr<typename CARD::ub_CrateHeader> _crateHeader;
	bool _createHeaderFromData;
  };
  
template <typename CARD>  
std::vector<CARD> const&  ub_MarkedRawCrateData<CARD>::getCards()
{ 
	if(!_isFullyDissected)
		dissectCards();
		
	  return _markedRawCardsData;
}

template <typename CARD>
void ub_MarkedRawCrateData<CARD>::dissectCards()
{
	try
	{
		dissector_type<CARD> dissector(data());
		dissector.populateCardDataVector(_markedRawCardsData);
		_isFullyDissected=true;		
		_dissectableDataSize=minsize()+dissector.getTrueDataSize();
		assert(_dissectableDataSize > minsize());
		assert(_dissectableDataSize <= rawdata().size());
	}
	catch(std::exception &ex)
	{
		throw;
	}
	catch(...)
	{
		throw std::runtime_error("Caught unknown exception in ub_MarkedRawCrateData::dissectCards()."); 
	}
}
	
template <typename CARD>  
bool ub_MarkedRawCrateData<CARD>::canFullyDissect()
{
	try
	{
		dissectCards();
	}
	catch(std::exception &ex)
	{
		std::cout << "Exception:" << ex.what() << std::endl;
		return false;
	}
	return true;
}

template <typename CARD>  
std::unique_ptr<typename CARD::ub_CrateHeader>& ub_MarkedRawCrateData<CARD>::crateHeader() 
{ 
  if(_crateHeader)
    return _crateHeader;
    
  assert(!getCards().empty());
  
  std::unique_ptr<typename CARD::ub_CrateHeader> crateHeader{
    new typename CARD::ub_CrateHeader(getCards().begin()->header())};
    
  _crateHeader.swap(crateHeader);
  
  return _crateHeader; 
}


template <typename CARD> 
std::string ub_MarkedRawCrateData<CARD>::debugInfo()const
{
	std::ostringstream os;
	os << "Object " << demangle(typeid(this)) << "."<< std::endl;
	os << header().debugInfo();
	os << trailer().debugInfo();
	
	os << " *Found " << std::dec << getCards().size() << " cards." << std::endl;
	for(auto card : getCards())
		os << card.debugInfo();
		
	//os <<  ub_MarkedRawDataBlock::debugInfo();
	return os.str();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 

#endif //_UBOONE_TYPES_MARKEDRAWCRATEDATA_H

