#include "ub_ChannelData.h"

#include <vector>
#include <bitset>
#include <fstream>
#include <stdexcept>

using namespace gov::fnal::uboone::datatypes;

void ub_ChannelData::CreateMarkedRawChannelData(){
  if(!_markedRawChannelData)
    _markedRawChannelData.swap(ub_MarkedRawChannelData::CreateMarkedRawChannelData(_version,_rawChannelData));
}

std::vector<ub_RawDataWord_t> const& ub_ChannelData::getDataVector(){
  CreateMarkedRawChannelData();
  ub_RawData_t const& data_iters = _markedRawChannelData->data();
  std::vector<ub_RawDataWord_t> const& dataVector(data_iters.begin(),data_iters.end());
  return dataVector;
}

const size_t CHANNEL_DATA_SIZE = 9595;

void ub_ChannelData::CreateUncompressedVector(){
  if(_uncompressedVector.size()!=0) return;

  _uncompressedVector.reserve(CHANNEL_DATA_SIZE);

  uint16_t las_uncompressed_word=0;
  std::vector<ub_RawData_Word_t> const& compressedVector = getDataVector();
  for(auto const& word : compressedVector){
    
    //if the word is not compressed
    if(word&0x8000==0){
      last_uncompressed_word = word&0x7fff;
      _uncompressedVector.push_back(last_uncompressed_word);
    }//end if uncompressed

    //else, we have a compressed word
    else{

      uint16_t outword;
      size_t zero_count = 0;
      bool   non_zero_found = false;

      //loop through word, checking bit by bit
      for(size_t index=0; index<16; ++index){
        
	//check to see if this bit is zero
	//if it is, count it
	if( !((word >> index) & 0x1) ) {
          if(non_zero_found) zero_count ++; // Count zeros IF we're past the padding in the right-hand bits.
        }
	//else, we have to put in the word
	else{
          if(!non_zero_found) non_zero_found= true;
          else {
            switch(zero_count) { // subst 
              case 0: outword = last_uncompressed_word;    break;
              case 1: outword = last_uncompressed_word -1; break;
              case 2: outword = last_uncompressed_word +1; break;
              case 3: outword = last_uncompressed_word -2; break;
              case 4: outword = last_uncompressed_word +2; break;
              case 5: outword = last_uncompressed_word -3; break;
              case 6: outword = last_uncompressed_word +3; break;
              default:
              // std::cout << "Huffman decompress unrecoginized bit pattern " << (std::bitset<16>) word << std::endl;
                throw std::runtime_error("Huffman decompress unrecoginized bit pattern");                
            }
            // std::cout << "huff out @ " << std::dec << uncompressed_vector.size() << "  0x" << std::hex << outword << std::endl;
            _uncompressedVector.push_back(outword);
            last_uncompressed_word = outword;   // Activite this line is delta is from last word. Comment out this line if diff is from the last EXPLICIT word, instead of the last huffman-compressed word.
            zero_count=0;
          }
        }
      }

    }//end else for compressed word

  }

}
