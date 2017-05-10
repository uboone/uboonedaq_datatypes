#include "ub_CardDataCreatorHelperClass.h"

// template override.
namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
// Supernova-specific variant, for testing.
  
  // void find_card_header(ub_RawData& in, )

template<>
void ub_CardDataCreatorHelperClass<ub_TPC_SN_CardData_v6>::populateCardDataVector(std::vector<ub_TPC_SN_CardData_v6> & cardDataVector)
{
  // std::cout << "***Running My Special Dissector Variant***" << std::endl;
  std::vector<ub_TPC_SN_CardData_v6> retValue;
  retValue.reserve(21);
    
  ub_RawData curr_rawData {_rawData.begin(),_rawData.end()-2};
  uint32_t card_raw_data_size;

  bool done = false;
  while(!done) {
    // std::cout << " //////////////////////// " << std::endl;
    // Let's run through looking for the FFFF header word for the next card, instead of relying on word count.
    if(curr_rawData.size() < ub_TPC_SN_CardData_v6::size_of_data_overhead()) {
      // std::cout << "Not enough data left to hold a card." << std::endl;
      cardDataVector.swap(retValue);
      _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
      return;
    }
    const ub_TPC_SN_CardData_v6::card_header_type& header = quick_cast<typename ub_TPC_SN_CardData_v6::card_header_type>(curr_rawData.begin());
    // std::cout << "Remaining: " << std::dec << curr_rawData.size() << std::endl;
    // std::cout << "Overhead:  " << std::dec << ub_TPC_SN_CardData_v6::size_of_data_overhead() << std::endl;
    // std::cout << "carddata:  " << header.getWordCount() << std::endl;

    // std::cout << "Card header:" << header.debugInfo() << std::endl;
    if(header.getMark() != 0xffff) throw(datatypes_exception("Bad card header. Mark incorrect!"));
    
    card_raw_data_size = ub_TPC_SN_CardData_v6::size_of_data_overhead() +
      header.getWordCount();
    
    // check that this either gets us to another marked card, or to the end of the data.
    bool cardsize_appears_correct = false;
    ub_RawData::const_iterator mark=curr_rawData.begin() + card_raw_data_size;
    if(mark > curr_rawData.end()) {
       cardsize_appears_correct = false;
       // std::cout << "Card size too large; out of bounds. Size " << card_raw_data_size << " remaining: " <<  curr_rawData.size() << std::endl;
    } else if(mark == curr_rawData.end()) {
       cardsize_appears_correct = true; 
    } else if( *mark == 0xFFFF ) {
      cardsize_appears_correct = true;  // Yup, looks like a card header!
    } else {
      cardsize_appears_correct = false;
      // std::cout << "Card size incorrect; no starting marker at position " << card_raw_data_size << " remaining: " <<  curr_rawData.size() << std::endl;
      
    }
    
    if(  cardsize_appears_correct ) {
      // std::cout << "Card header size is correct." << std::endl;
    } else {
      // OK, let's look by hand.
      // std::cout << "Card header has wrong size." << std::endl;
      mark = curr_rawData.begin() + size_of<ub_TPC_SN_CardData_v6::card_header_type>();
      bool found_next_card = false;
      while(mark != curr_rawData.end()) {
        if(*mark == 0xFFFF ) {
          found_next_card = true;
          // check the next 11 words, they should all start with 0xf too.
          for(size_t i=1;i<12 && (mark+i)!=curr_rawData.end();i++) {
            if( ((*(mark+i))&0xf000) != 0xf000) {
              found_next_card = false;
              break;
            }
          }
          if(found_next_card) break;
        }
        mark++;
      }
      if(!found_next_card) {
        // std::cout << "Couldn't find next card header!" << std::endl;
        mark =  curr_rawData.end();
      } else {
        // std::cout << "Found header at position " << mark- curr_rawData.begin() << std::endl;
      }
      
      // std::cout << "Card header should be " << mark-curr_rawData.begin() << " was " << card_raw_data_size << " diff " << mark-curr_rawData.begin() - card_raw_data_size << std::endl;
    }
    
    // ub_RawData this_card_data{curr_rawData.begin(),curr_rawData.begin()+card_raw_data_size-1};
    ub_RawData this_card_data{curr_rawData.begin(),mark};
    retValue.emplace_back(this_card_data);
    curr_rawData = ub_RawData{mark,curr_rawData.end()};
    if ( *( mark -1 ) == 0xe000 && *( mark -2 ) == 0x0000 ) {
      cardDataVector.swap(retValue);
      _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
      // std::cout << "Find 0000 e000" << std::endl;
      return;
    }
    else if ( *( mark +1 ) == 0xffff && *( mark +2 ) == 0xffff ) {
      cardDataVector.swap(retValue);
      _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
      // std::cout << "Find ffff ffff in the next words" << std::endl;
      return;
    }
  }
    

}

}}}}
