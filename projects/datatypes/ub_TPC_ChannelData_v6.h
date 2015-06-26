#ifndef _UBOONE_TYPES_TPCCHANELDATA_V6_H
#define _UBOONE_TYPES_TPCCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_TPC_ChannelHeaderTrailer_v0.h"
#include <bitset>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_ChannelData_v6 final: public ub_MarkedRawChannelData<ub_TPC_ChannelHeader,ub_TPC_ChannelTrailer>
{
public:
    ub_TPC_ChannelData_v6(ub_RawData const& rawdata);
    ub_TPC_ChannelData_v6 ( ub_TPC_ChannelData_v6 && ) = default;

    uint16_t getChannelHeaderWord() const noexcept;
    uint16_t getChannelTrailerWord() const noexcept;

    /// Huffman decompression. Available as T= uint_16, int16, uint_32, int32, float, and double. 
    template<typename T>
     void decompress(std::vector<T>& uncompressed) const throw(datatypes_exception);  

};


template<typename T>
void ub_TPC_ChannelData_v6::decompress(std::vector<T>& uncompressed) const throw(datatypes_exception)
{
  /// 
  /// Huffman decompression.
  ///
  /// Fills the supplied vector with huffman-decompressed data.
  /// Note that the user should check the vector to make sure that the size is what is expected (e.g. size()==9600)
  /// Won't throw if output size is wrong
  /// WILL throw if there's an unrecognized bit pattern.
  ///
  /// Available as T= uint_16, int16, uint_32, int32, float, and double. 
  /// Why make so many types available? Because it's a needless extra expense in both memory and 
  /// cpu time to make a copy.  Online monitor, for instance, likes to use doubles because they translate well
  /// into the TH1::Fill commands.  
  ///
  /// 
  /// N. Tagg. May 2015
  ///

  //  1 00001 001 000

  // This saves a fair bit of time in the long run.
  // It's much faster to resize() and use operator[] than to reserve, but this is the safest in case of buffer overruns.
  //
  const size_t kMaxBufferSize = 9600;
  // std::cout << debugInfo();
  // uncompressed.resize(9600); // Set size.
  
  uncompressed.resize(0); // Set size.
  uncompressed.reserve(kMaxBufferSize);  // Enlarge if buffer is too small. No effect if buffer is already large.

  const ub_RawData& raw = data();
  ub_RawData::const_iterator it;
  uint16_t last_uncompressed_word = 0;
  //size_t   outpos = 0;

  
  // Dump check for debugging.
  for(it = raw.begin(); it!= raw.end(); it++) {

    uint16_t word = *it;
    //if it's not a compressed word, just put it on the uncompressed vector
    // std::cout << "\n\n";
    std::bitset<16> bits(word);
    // std::cout << bits << std::endl;
    if( (word & 0x8000)==0 ) {
      last_uncompressed_word = word&0xfff;
      // explicit conversion to templated type. Use push_back if size to small, otherwise [] is faster.
      // if(outpos >= uncompressed.size()) uncompressed.push_back((T)(last_uncompressed_word));
      // else uncompressed[outpos] = (  (T)(last_uncompressed_word)  );
      // outpos++;
      uncompressed.push_back((T)(last_uncompressed_word));
      // std::cout << "\n  u" << hex(4,word);
    } else {  // huffman bit on.
      // std::cout << "\n  h" << hex(4,word);
      
      // Padding in LSB
      // top 2 bits are "header" and always 10
      // Bit patters are 1,01,001,0001,00001 etc.
      // Example: 1000 0000 1110 0000
      //          xxcc cccc cbap pppp
      // p = padding
      // a -> increment zero
      // b -> increment zero 
      // c -> +3 adc counts.
      
      uint16_t outword;
      size_t zero_count = 0;
      bool   non_zero_found = false;

      // set terminating bit in bit 14.
      // This is a hack to allow the end-of-pattern to be found.
      // We COULD rewrite everything, but this is easier right now.
      word = word | (1<<14);

      // Then read everything up to and including bit 14.
      for(size_t index=0; index<15; ++index){
        if( (word & 0x1)==0 ) { // is this bit a zeron?
          if(non_zero_found) zero_count ++; // Count zerons IF we're past the padding in the right-hand bits.
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
              // std::cout << "\n----\n";
              // std::cout << debugInfo();
              std::stringstream ss;
              ss << "Huffman decompress unrecoginized bit pattern:" << hex(4,*it) << " on word number " << (it-raw.begin());
                throw datatypes_exception(ss.str());
            }

            // if(outpos >= uncompressed.size()) uncompressed.push_back((T)(outword));
            // else uncompressed[outpos] = (  (T)(outword)  );
            // outpos++;
            // std::cout << " out :" << hex(4,outword);
            uncompressed.push_back((T)(outword));

            last_uncompressed_word = outword;   // Activite this line is delta is from last word. Comment out this line if diff is from the last EXPLICIT word, instead of the last huffman-compressed word.
            zero_count=0;
          }
        }

        word = word >> 1; // Shift a bit to look at the next one.      
      }      


      
    } //end else huffman bit on
  }//end for loop over data words
  
  // uncompressed.resize(outpos);


} 

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
