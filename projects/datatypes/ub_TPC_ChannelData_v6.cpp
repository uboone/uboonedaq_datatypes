#include "ub_TPC_ChannelData_v6.h"

using namespace gov::fnal::uboone::datatypes;

ub_TPC_ChannelData_v6::ub_TPC_ChannelData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}

const size_t kMaxBufferSize = 9600;

template<typename T>
std::shared_ptr< std::vector<T> > ub_TPC_ChannelData_v6::decompress() const throw(datatypes_exception)
{
  /// 
  /// Huffman decompression.
  /// Available as T= uint_16, int16, uint_32, int32, float, and double. 
  /// Why make so many types available? Because it's a needless extra expense in both memory and 
  /// cpu time to make a copy.  Online monitor, for instance, likes to use doubles because they translate well
  /// into the TH1::Fill commands.  
  /// N. Tagg. May 2015
  ///

  uint i_bit,i_zero; // some counters we will use

  std::shared_ptr< std::vector<T> > uncompressed_vector(new std::vector<T>);

  uncompressed_vector->reserve(kMaxBufferSize);  
  // It's much faster to resize() and use operator[] than to reserve, but this is the safest in case of buffer overruns.
  
  const ub_RawData& raw = data();
  ub_RawData::const_iterator it;
  uint16_t last_uncompressed_word = 0;

  // Dump check for debugging.
  // std::ofstream of("hufform.dump");

  for(it = raw.begin(); it!= raw.end(); it++) {

    if(uncompressed_vector->size()>kMaxBufferSize) {
      throw(datatypes_exception("Huffmann decompress resulted in too-large output buffer."));
    }
    uint16_t word = *it;
    // of << iword << "\t" << word << std::endl; // Dump for debugging.

    //if it's not a compressed word, just put it on the uncompressed vector
    if( (word & 0x8000)==0 ) {
      // std::cout << "uncompressed word " << std::hex << word << std::endl;
      last_uncompressed_word = word&0x7ff;
      //std::cout << "huff out @ " << std::dec << uncompressed_vector.size() << "  0x" << std::hex << last_uncompressed_word << std::endl;
      uncompressed_vector->push_back( (T) (last_uncompressed_word) ); // explicit conversion to templated type
    
    } else {  // huffman bit on.
       // std::cout << "compressed word 0x" << std::hex << word << " b" << (std::bitset<16>) word << std::endl;


      uint16_t outword;
      size_t zero_count = 0;
      bool   non_zero_found = false;
      for(size_t index=0; index<16; ++index){
        if( !((word >> index) & 0x1) ) {
          if(non_zero_found) zero_count ++; // Count zeros IF we're past the padding in the right-hand bits.
        }else{
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
            uncompressed_vector->push_back( (T)(outword) );
            last_uncompressed_word = outword;   // Activite this line is delta is from last word. Comment out this line if diff is from the last EXPLICIT word, instead of the last huffman-compressed word.
            zero_count=0;
          }
        }
      }
      
    } //end else huffman bit on
  }//end for loop over data words

  return uncompressed_vector;
 } 