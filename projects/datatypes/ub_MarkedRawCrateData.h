#ifndef _UBOONE_TYPES_MARKEDRAWCRATEDATA_H
#define _UBOONE_TYPES_MARKEDRAWCRATEDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_EventHeaderTrailer_v0.h"
#include "ub_CardDataCreatorHelperClass.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
	
template <typename CARD> class ub_MarkedRawCrateData : 
public ub_MarkedRawDataBlock<ub_EventHeader,ub_EventTrailer>{
  public:
	template <typename MRCD> using dissector_type = ub_CardDataCreatorHelperClass<MRCD>;
    
	explicit ub_MarkedRawCrateData(ub_RawData const rawdata): 
	ub_MarkedRawDataBlock<ub_EventHeader,ub_EventTrailer>(rawdata),
	_markedRawCardsData{},_isValid{isValid()},_isFullyDissected{canFullyDissect()}{}

    uint32_t const& getHeaderWord() const {return header().raw_data; } 
	uint32_t const& getTrailerWord() const {return trailer().raw_data; } 
	std::vector<CARD> const&  getCards() ;
	std::vector<CARD> const&  getCards() const {return _markedRawCardsData;}
		
	ub_MarkedRawCrateData() = delete;
	//ub_MarkedRawCrateData(ub_MarkedRawCrateData const &) = delete;
	ub_MarkedRawCrateData& operator=(ub_MarkedRawCrateData const &) = delete;
	size_t getCrateDataSize() const{return rawdata().size();};

	void dissectCards();
	std::string debugInfo()const;
private:
	bool isValid();
	bool canFullyDissect();	
private:
	std::vector<CARD> _markedRawCardsData;	
	bool _isValid;
	bool _isFullyDissected;
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

