#ifndef _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<typename MRCD>
class ub_CardDataCreatorHelperClass final{
public:
    ub_CardDataCreatorHelperClass(ub_RawData const& rd) :_rawData(rd),_dissectableDataSize(0) {}
    void populateCardDataVector(std::vector<MRCD> & cardDataVector) ;
    size_t getTrueDataSize() const noexcept{
        return _dissectableDataSize;
    }

private:
    const ub_RawData _rawData;
    size_t _dissectableDataSize;
};

template<typename MRCD>
void ub_CardDataCreatorHelperClass<MRCD>::populateCardDataVector(std::vector<MRCD> & cardDataVector)
{
    ub_RawData curr_rawData {_rawData};
    std::vector<MRCD> retValue;
    uint32_t card_raw_data_size;
    
    int counter{0};
    bool end_of_event{false};
    try{
    while ( curr_rawData.size() > MRCD::size_of_data_overhead() )
    {   
        counter++;
        card_raw_data_size = MRCD::size_of_data_overhead() +
                             quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).getWordCount();
          
	//This part is for handling bad word counts due to missing 503f.                   
	//Really wish we didn't have to do this ... should be removed when firmware is fixed.
	// --Wes, 06/15/15
        if(card_raw_data_size-1 > curr_rawData.size()){
            std::stringstream ss;
	    ss << "Junk data: Wrong word count in the card header.";
	    ss << "\tCardSize-1=" << card_raw_data_size-1 << " , RemainingSize=" << curr_rawData.size();
            throw datatypes_exception(ss.str());
	}
	if( *(curr_rawData.begin()+card_raw_data_size-1)==0xffff ){
	  std::stringstream ss;
	  ss << "PROBABLY Wrong word count in the card header.";
	  ss << "\nFinal word in data was 0xffff.";
	  ss << "\nRewinding by one word, and praying that we land in the right spot for further processing.";
	  std::cerr << ss.str() << std::endl;
	  card_raw_data_size = card_raw_data_size-1;
	}
	else if( quick_cast<uint32_t>(curr_rawData.begin()+card_raw_data_size-1)==EVENTTRAILER ){
	  std::stringstream ss;
	  ss << "PROBABLY Wrong word count in the card header.";
	  ss << "\nFinal word in data was 0xe0000000.";
	  ss << "\nRewinding by one word, and praying that we land in the right spot for further processing.";
	  std::cerr << ss.str() << std::endl;
	  card_raw_data_size = card_raw_data_size-1;
	}
	
	//this if is the normal check for the raw data size. This should remain even when above is moved out.
	if(card_raw_data_size > curr_rawData.size()){
            std::stringstream ss;
	    ss << "Junk data: Wrong word count in the card header.";
	    ss << "\tCardSize=" << card_raw_data_size << " , RemainingSize=" << curr_rawData.size();
            throw datatypes_exception(ss.str());
	}

        ub_RawData data {curr_rawData.begin(),curr_rawData.begin()+card_raw_data_size};

        retValue.emplace_back(data);
        /*
	std::stringstream ss;
	ss << "Card " << counter << ", Size=" << card_raw_data_size;
	ss << "\tLast word=0x" << std::hex << *(curr_rawData.begin()+card_raw_data_size-1);
	ss << "\tNext word=0x" << std::hex << *(curr_rawData.begin()+card_raw_data_size);
	*/
        curr_rawData=ub_RawData {curr_rawData.begin()+card_raw_data_size,curr_rawData.end()};
        
	//handle padding zeroes
	//Note from Wes: padded zeroes do not get added to cardDataVector!
	//I think this is the right thing to do, but we can revisit.
	while(quick_cast<uint16_t>(curr_rawData.begin())==0x0000){
	  if (quick_cast<uint32_t>(curr_rawData.begin())==EVENTTRAILER){
	    end_of_event=true;
            break;
	  }
	  curr_rawData=ub_RawData{curr_rawData.begin()+1,curr_rawData.end()};
	}
	/*
	ss << "\tFinal word=0x" << std::hex << *(curr_rawData.begin()-1);	
	ss << "\tNext word=0x" << std::hex << *(curr_rawData.begin());	
	std::cerr << ss.str() << std::endl;
	*/
	if(end_of_event)
	  break;

	

    }
    _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
    cardDataVector.swap(retValue);
    }catch(std::exception& e){
         std::cerr <<  "Caught exception in ub_CardDataCreatorHelperClass::populateCardDataVector() Message: " <<e.what() << std::endl;
         std::cerr <<  "Details: Card number" << counter << std::endl;
         std::cerr <<  quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).debugInfo() << std::endl;
        // std::cerr <<  debugInfoShort(curr_rawData) << std::endl;

	 std::cerr << "\n\nFullDataBlock!" << std::endl;
	 std::cerr << debugInfo(_rawData) << std::endl;
        throw;
    }
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
