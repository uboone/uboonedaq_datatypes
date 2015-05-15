#ifndef _UBOONE_TYPES_TPCCHANELDATA_V6_H
#define _UBOONE_TYPES_TPCCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_TPC_ChannelHeaderTrailer_v0.h"

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

  // This saves a fair bit of time in the long run.
  // It's much faster to resize() and use operator[] than to reserve, but this is the safest in case of buffer overruns.
  //
  const size_t kMaxBufferSize = 9600;
  
  // uncompressed.resize(9600); // Set size.
  
  uncompressed.resize(0); // Set size.
  uncompressed.reserve(kMaxBufferSize);  // Enlarge if buffer is too small. No effect if buffer is already large.

  const ub_RawData& raw = data();
  ub_RawData::const_iterator it;
  uint16_t last_uncompressed_word = 0;
  size_t   outpos = 0;

  
  // Dump check for debugging.
  for(it = raw.begin(); it!= raw.end(); it++) {

    const uint16_t& word = *it;
    //if it's not a compressed word, just put it on the uncompressed vector
    if( (word & 0x8000)==0 ) {
      last_uncompressed_word = word&0x7ff;
      // explicit conversion to templated type. Use push_back if size to small, otherwise [] is faster.
      // if(outpos >= uncompressed.size()) uncompressed.push_back((T)(last_uncompressed_word));
      // else uncompressed[outpos] = (  (T)(last_uncompressed_word)  );
      // outpos++;
      uncompressed.push_back((T)(last_uncompressed_word));
      
    } else {  // huffman bit on.
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
                throw datatypes_exception("Huffman decompress unrecoginized bit pattern");                
            }
            
            // if(outpos >= uncompressed.size()) uncompressed.push_back((T)(outword));
            // else uncompressed[outpos] = (  (T)(outword)  );
            // outpos++;
            uncompressed.push_back((T)(outword));

            last_uncompressed_word = outword;   // Activite this line is delta is from last word. Comment out this line if diff is from the last EXPLICIT word, instead of the last huffman-compressed word.
            zero_count=0;
          }
        }
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
