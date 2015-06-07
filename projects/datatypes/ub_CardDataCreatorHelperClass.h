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
    try{
    while ( curr_rawData.size() > MRCD::size_of_data_overhead() )
    {   
        counter++;
        card_raw_data_size = MRCD::size_of_data_overhead() +
                             quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).getWordCount();
                             
        if(card_raw_data_size > curr_rawData.size()){
            std::stringstream ss;
	    ss << "Junk data: Wrong word count in the card header.";
	    ss << "\tCardSize=" << card_raw_data_size << " , RemainingSize=" << curr_rawData.size();
            throw datatypes_exception(ss.str(),"datatype_exception",MRCD::typeName,-1,quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).getModule(),0);
	}
        ub_RawData data {curr_rawData.begin(),curr_rawData.begin()+card_raw_data_size};

        retValue.emplace_back(data);
        
        curr_rawData=ub_RawData {curr_rawData.begin()+card_raw_data_size,curr_rawData.end()};
        
        if (quick_cast<uint32_t>(curr_rawData.begin())==EVENTTRAILER)
            break;
    }
    _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
    cardDataVector.swap(retValue);
    }catch(std::exception& e){
         std::cerr <<  "Caught exception in ub_CardDataCreatorHelperClass::populateCardDataVector() Message: " <<e.what() << std::endl;
         std::cerr <<  "Details: Card number" << counter << std::endl;
         std::cerr <<  quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).debugInfo() << std::endl;
        // std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
        throw;
    }
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
