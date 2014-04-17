#include "card_utilities.h"
#include "datatypes/constants.h"
#include <stdexcept>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

long utilities::compareCheckSum(cardHeader const& cH, cardData const& cD){

  //easy part: get the word from the header
  long header_value = (long)(cH.getChecksum());

  //harder part: get the word from the data
  //first, check the IO mode
  const uint8_t io_mode = cD.getIOMode();

  long data_value = 0;
  if(io_mode == IO_GRANULARITY_CARD)
    data_value = (long)(getCheckSumFromDataBlock(cD.getCardDataPtr(),cD.getCardDataSize()));
  

  else if(io_mode == IO_GRANULARITY_CHANNEL){

    uint32_t data_sum = 0;
    for(auto channel : cD.getChannelMap())
      data_sum += getCheckSumFromChannel(channel.second);
    data_sum = data_sum & 0x00ffffff;
    
    data_value = (long)(data_sum);

  }

  else{
    throw std::runtime_error("gov::fnal::uboone::datatypes::compareCheckSum ERROR! Unworkable IO mode.");
    return -9999;
  }

  return (header_value - data_value);

}


long utilities::compareCheckSum(cardHeaderPMT const& cH, cardDataPMT const& cD){

  //easy part: get the word from the header
  long header_value = (long)(cH.getChecksum());

  //harder part: get the word from the data
  //first, check the IO mode
  const uint8_t io_mode = cD.getIOMode();

  long data_value = 0;
  if(io_mode == IO_GRANULARITY_CARD)
    data_value = (long)(getCheckSumFromDataBlock(
        cD.getCardDataPtr(),
        cD.getCardDataSize() -2 // Via Kazu: Ignore the last word of data; the trailer word doesn't count.
          ));
  

  else if(io_mode == IO_GRANULARITY_CHANNEL){

    uint32_t data_sum = 0;
    for(auto channel : cD.getChannelMap())
      data_sum += getCheckSumFromChannel(channel.second);
    data_sum = data_sum & 0x00ffffff;
    
    data_value = (long)(data_sum);

  }

  else{
    throw std::runtime_error("gov::fnal::uboone::datatypes::compareCheckSum ERROR! Unworkable IO mode.");
    return -9999;
  }

  return (header_value - data_value);

}

uint32_t utilities::getCheckSumFromDataBlock(const char* data_ptr, size_t data_size){
  
  uint32_t checksum_value = 0;

  size_t n_16bit_words = data_size / sizeof(uint16_t) ; 
  const uint16_t* data_ptr_16bit = reinterpret_cast<const uint16_t*>(data_ptr);
  for(size_t i=0; i<n_16bit_words; i++)
    checksum_value += *(data_ptr_16bit + i);
  
  checksum_value = checksum_value & 0x00ffffff;

  return checksum_value;

}

uint32_t utilities::getCheckSumFromChannel(channelData const& chD){

  uint32_t checksum_value = 0;

  checksum_value += chD.getChannelHeader();
  
  size_t n_16bit_words = chD.getChannelDataSize() / sizeof(uint16_t);
  const uint16_t* data_ptr_16bit = reinterpret_cast<const uint16_t*>(chD.getChannelDataPtr());
  for(size_t i=0; i<n_16bit_words; i++)
    checksum_value += *(data_ptr_16bit + i);
  
  checksum_value += chD.getChannelTrailer();

  return checksum_value;

}


uint32_t utilities::getCheckSumFromChannel(channelDataPMT const& chD)
{

  assert(0);  // FIXME!
// 
//   uint32_t checksum_value = 0;
// 
//   checksum_value += chD.getChannelHeader();
//   
//   size_t n_16bit_words = chD.getChannelDataSize() / sizeof(uint16_t);
//   const uint16_t* data_ptr_16bit = reinterpret_cast<const uint16_t*>(chD.getChannelDataPtr());
//   for(size_t i=0; i<n_16bit_words; i++)
//     checksum_value += *(data_ptr_16bit + i);
//   
//   checksum_value += chD.getChannelTrailer();
// 
//   return checksum_value;
// 
}

}}}} // namespaceage
