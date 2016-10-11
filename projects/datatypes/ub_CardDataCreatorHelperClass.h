#ifndef _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_TPC_SN_CardData_v6.h"

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

    void throw_data_exception_Junk_Word_Count(size_t data_size_exp,size_t data_size_act);
    void throw_padding_exception();
    void emplace_data(ub_RawData const& data,size_t data_size,
		      std::vector<MRCD>& retValue);

    bool has_16bits_at_end(ub_RawData const& data,size_t card_raw_data_size);
    bool word_at_position_is_zero(ub_RawData const& data,size_t pos);
    bool word_at_position_is_E000(ub_RawData const& data,size_t pos);
    bool word_at_position_is_FFFF(ub_RawData const& data,size_t pos);

};

template<typename MRCD>
void ub_CardDataCreatorHelperClass<MRCD>::throw_data_exception_Junk_Word_Count(size_t data_size_exp,size_t data_size_act){
  std::stringstream ss;
  ss << "Junk data: Wrong word count in the card header.";
  ss << "\tExpSize=" << data_size_exp << " , ActualSize=" << data_size_act;
  std::cerr << ss.str() << std::endl;
  throw data_size_exception(data_size_exp-data_size_act,ss.str());
}

template<typename MRCD>
void ub_CardDataCreatorHelperClass<MRCD>::throw_padding_exception(){
  std::stringstream ss;
  ss << "Junk data: Too many padding words detected.";
  std::cerr << ss.str() << std::endl;
  throw datatypes_exception(ss.str());
}

template<typename MRCD>
void ub_CardDataCreatorHelperClass<MRCD>::emplace_data(ub_RawData const& data,size_t data_size,
		  std::vector<MRCD>& retValue)
{  
  ub_RawData new_data { data.begin(),data.begin()+data_size};
  retValue.emplace_back (new_data);
}

template<typename MRCD>
bool ub_CardDataCreatorHelperClass<MRCD>::has_16bits_at_end(ub_RawData const& data,size_t card_raw_data_size)
{ return (card_raw_data_size<=data.size()); }

template<typename MRCD>
bool ub_CardDataCreatorHelperClass<MRCD>::word_at_position_is_zero(ub_RawData const& data,size_t pos)
{ return *(data.begin()+pos)==0x0000; }

template<typename MRCD>
bool ub_CardDataCreatorHelperClass<MRCD>::word_at_position_is_E000(ub_RawData const& data,size_t pos)
{  return *(data.begin()+pos)==0xe000; }

template<typename MRCD>
bool ub_CardDataCreatorHelperClass<MRCD>::word_at_position_is_FFFF(ub_RawData const& data,size_t pos)
{ return *(data.begin()+pos)==0xffff; }

template<typename MRCD>
void ub_CardDataCreatorHelperClass<MRCD>::populateCardDataVector(std::vector<MRCD> & cardDataVector)
{
  std::vector<MRCD> retValue;
  retValue.reserve(21);
    
  ub_RawData curr_rawData {_rawData};
  uint32_t card_raw_data_size;
    
  int counter{0};
  const int max_padding_words{8};
  bool end_of_event{false};
  try{
    while ( curr_rawData.size() > MRCD::size_of_data_overhead() )
    {   
      counter++;
      std::cout << "Overhead: " << std::dec << MRCD::size_of_data_overhead() << std::endl;
      std::cout << "carddata: " << quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).getWordCount() << std::endl;
      card_raw_data_size = MRCD::size_of_data_overhead() +
        quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).getWordCount();

      if ( handle_missing_words<MRCD>() ){

        if(! this->has_16bits_at_end(curr_rawData,card_raw_data_size)){

          if ( !(this->has_16bits_at_end(curr_rawData,card_raw_data_size-1))||
            peek_at_next_event<MRCD>() )       //for reading circular buffer, or missing more than one word on end
          {
            this->throw_data_exception_Junk_Word_Count(card_raw_data_size-1,curr_rawData.size());
          }
          else{                                   //for reading from processed fragment
            this->emplace_data(curr_rawData,card_raw_data_size-1,retValue);
            curr_rawData=ub_RawData{curr_rawData.begin()+card_raw_data_size-1,curr_rawData.end()};
            end_of_event = true;
            break; //done. end of event. we hit the end of the fragment
          }

        }

	
        int padding_words = 0;
        while(padding_words < max_padding_words){

          //check if we have data to read
          if( ! this->has_16bits_at_end(curr_rawData,card_raw_data_size+padding_words) ){
            if(peek_at_next_event<MRCD>() )
              this->throw_data_exception_Junk_Word_Count(card_raw_data_size+padding_words,curr_rawData.size());
            else{
              this->emplace_data(curr_rawData,card_raw_data_size,retValue); //don't add padding words to data...
              curr_rawData=ub_RawData{curr_rawData.begin()+card_raw_data_size+padding_words-1,curr_rawData.end()};
              end_of_event = true;
              break; //done. end of event. we hit the end of the fragment
            }
          }
          else if(! this->word_at_position_is_zero(curr_rawData,card_raw_data_size+padding_words-1))
            break;

          padding_words++;
        }

        if(end_of_event) break;
        if(padding_words==max_padding_words)
          throw_padding_exception();

        if( this->word_at_position_is_E000(curr_rawData,card_raw_data_size+padding_words-1) ){
          this->emplace_data(curr_rawData,card_raw_data_size-1,retValue);
          curr_rawData=ub_RawData{curr_rawData.begin()+card_raw_data_size+padding_words,curr_rawData.end()};
          end_of_event = true;
          break;
        }
        else if( this->word_at_position_is_FFFF(curr_rawData,card_raw_data_size+padding_words-1) ){
          this->emplace_data(curr_rawData,card_raw_data_size-1,retValue);
          curr_rawData=ub_RawData{curr_rawData.begin()+card_raw_data_size+padding_words-1,curr_rawData.end()};

          if( ! this->has_16bits_at_end(curr_rawData,2) ){
            if(peek_at_next_event<MRCD>() )
              this->throw_data_exception_Junk_Word_Count(2,curr_rawData.size());
            else
              end_of_event = true;
          }
          else{
            if( this->word_at_position_is_FFFF(curr_rawData,1) )
              end_of_event = true;
          }

          if(end_of_event) break;
          else continue;

        }
        if(end_of_event) break;

      }//end if handle_missing_words<MRCD>()

      //this if is the normal check for the raw data size. This should remain even when above is moved out.
      if(card_raw_data_size > curr_rawData.size())
        this->throw_data_exception_Junk_Word_Count(card_raw_data_size,curr_rawData.size());

      ub_RawData data {curr_rawData.begin(),curr_rawData.begin()+card_raw_data_size};
      MRCD card(data);
      retValue.emplace_back(data);
      std::cout << "Got card " << (int)( quick_cast<typename MRCD::card_header_type>(data.begin()).getModule() )  << std::endl;
      curr_rawData=ub_RawData {curr_rawData.begin()+card_raw_data_size,curr_rawData.end()};

      if( handle_missing_words<MRCD>() ){

        int padding_words = 0;
        while(padding_words < max_padding_words){

          //check if we have data to read
          if( ! this->has_16bits_at_end(curr_rawData,padding_words+1) ){
            if(peek_at_next_event<MRCD>() )
              this->throw_data_exception_Junk_Word_Count(padding_words+1,curr_rawData.size());
            else{
              curr_rawData=ub_RawData{curr_rawData.begin()+padding_words+1,curr_rawData.end()};
              end_of_event = true;
              break; //done. end of event. we hit the end of the fragment
            }
          }
          else if(! this->word_at_position_is_zero(curr_rawData,padding_words))
            break;

          padding_words++;
        }
        if(end_of_event) break;
        if(padding_words==max_padding_words)
          throw_padding_exception();

        if( this->word_at_position_is_E000(curr_rawData,padding_words) ){
          curr_rawData=ub_RawData{curr_rawData.begin()+padding_words+1,curr_rawData.end()};
          end_of_event = true;
          break;
        }
        else if( this->word_at_position_is_FFFF(curr_rawData,padding_words) ){
          curr_rawData=ub_RawData{curr_rawData.begin()+padding_words,curr_rawData.end()};

          if( ! this->has_16bits_at_end(curr_rawData,2) ){
            if(peek_at_next_event<MRCD>() )
              this->throw_data_exception_Junk_Word_Count(2,curr_rawData.size());
            else
              end_of_event = true;
          }
          else{
            if( this->word_at_position_is_FFFF(curr_rawData,1) )
              end_of_event = true;
          }
	  
          if(end_of_event) break;
          else continue;

        }
        if(end_of_event) break;

      }

    }
    _dissectableDataSize=std::distance(_rawData.begin(),curr_rawData.begin());
        
    cardDataVector.swap(retValue);
  }
  catch(data_size_exception& e){
    throw;
  }
  catch(std::exception& e){
    std::ostringstream os; 
    os <<  "Caught exception in ub_CardDataCreatorHelperClass::populateCardDataVector() Message: " <<e.what() << std::endl;
    os <<  "Details: Card number" << counter << std::endl;
    os <<  quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).debugInfo() << std::endl;
    // std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
    std::cerr << os.str() << std::endl;	
    std::cerr << "\n\nFullDataBlock!" << std::endl;
    std::cerr << debugInfo(_rawData) << std::endl;
    throw datatypes_exception(os.str());
  }
  catch(...){
    std::ostringstream os; 
    os <<  "Caught unknown exception in ub_CardDataCreatorHelperClass::populateCardDataVector() " << std::endl;
    os <<  "Details: Card number" << counter << std::endl;
    os <<  quick_cast<typename MRCD::card_header_type>(curr_rawData.begin()).debugInfo() << std::endl;
    // std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
    std::cerr << os.str() << std::endl;
    std::cerr << "\n\nFullDataBlock!" << std::endl;
    std::cerr << debugInfo(_rawData) << std::endl;
    throw datatypes_exception(os.str());
  }    
}


 
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
