#ifndef _UBOONE_TYPES_TPC_SN_WINDOWDATA_V6_H
#define _UBOONE_TYPES_TPC_SN_WINDOWDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_TPC_SN_ChannelHeaderTrailer_v0.h"
#include "empty_trailer.h"
#include <bitset>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


struct ub_TPC_SN_PacketHeader final
{
  uint16_t header_word;    

    bool isCarryOverFromLastFrame() const noexcept { 
      return ((header_word&0xc000) != 0x4000); 
    }

    uint16_t getSampleNumber() const noexcept{
      // Special case: if it's the first sample, it might not have a header. 
      // The first sample's header is the same as the frame header, which begins with 0x4???
      if(isCarryOverFromLastFrame()) return 0;
      return header_word&0x3FFF;
    }
    uint16_t getHeaderWord() const noexcept{
        return header_word;
    }
    std::string debugInfo()const noexcept;
};



static_assert (sizeof(ub_TPC_SN_PacketHeader) == 2, "ub_TPC_SN_PacketHeader structure size is not correct.");


class ub_TPC_SN_PacketData_v6 final: public ub_MarkedRawChannelData<ub_TPC_SN_PacketHeader,empty_trailer>
{
public:
    ub_TPC_SN_PacketData_v6(ub_RawData const& rawdata);
    ub_TPC_SN_PacketData_v6 ( ub_TPC_SN_PacketData_v6 && ) = default;

    bool roiIsFinished() {
      // This function returns true if there is a trailer word (which I'm not putting in the trailer, because it might not exist)
      // If the trailer word isn't there, it means that the ROI continues in a packet in the next frame.
      if(data().size()>0) {
        // Check high nibble of the last word.  If it matches 0x3??? then it's a complete packet.
        return ((*(data().end()-1))&0xf000) == 0x3000;
      }
      return true;
    }

    template<typename T>
    void decompress_into(std::vector<T>& uncompressed) const throw(datatypes_exception)
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
      /// N. Tagg. Aug 2016
      ///
      
      /// Check for valid header.
      if(header().getSampleNumber()>=9600) throw datatypes_exception(std::string("ub_TPC_SN_PacketData_v6::decompress_into() Sample number too large:" + std::to_string(header().getSampleNumber())));
      
      uncompressed.resize(header().getSampleNumber()); // Set start.
      auto end_point = data().end(); // last word is special
      const ub_RawData& raw{data().begin(),end_point};
      ub_RawData::const_iterator it;
      uint16_t last_uncompressed_word = 0;

      for(it = raw.begin(); it!= raw.end(); it++) {
        uint16_t word = *it;
        //if it's not a compressed word, just put it on the uncompressed vector
    
        // Check top nibble for uncompressed word. A '1' in the MSB indicates a huffman word.
        if( (word & 0x8000)==0 ) {
          last_uncompressed_word = word&0xfff;
          // explicit conversion to templated type. Use push_back if size to small, otherwise [] is faster.
          // if(outpos >= uncompressed.size()) uncompressed.push_back((T)(last_uncompressed_word));
          // else uncompressed[outpos] = (  (T)(last_uncompressed_word)  );
          // outpos++;
          uncompressed.push_back((T)(last_uncompressed_word));
          if((word&0xF000) == 0x3000) {
            if(it!=raw.end()-1) {
              std::cout << "Packet trailer found before end-of-packet." << std::endl;
            }
          }
          
          // std::cout << "\n  u" << hex(4,word);
        } else {  // huffman bit on.
          if(word ==0) continue; // padding word.
          
          if((word&0x8000)!=0x8000) {
            std::stringstream ss;
            ss << "ub_TPC_SN_PacketData_v6::decompress_into() error: huffmann bit not set " << std::hex << word;
            throw datatypes_exception( ss.str());      
          }
      
          // Padding in LSB
          // top 2 bits are "header" and always 10
          // Bit patterns are 1,01,001,0001,00001 etc.
          // Example: 1000 0000 1110 0000
          //          xxcc cccc cbap pppp
          // p = padding
          // a -> increment zero
          // b -> increment zero 
          // c -> +3 adc counts.
      

          // There is already a terminating bit in bit 16, so we don't need to adjust anything.      
          // The idea here is:
          // 100010001000000 has padding.
          // Strip off the zeroes on the end to get:
          // 100010001    This has got a pattern on the end.
          //      ---- < bit pattern 0001
          // 10001000
          //     ----  < look for bit pattern 1000 instead
      
          if     ((word & 0x1   ) == 0x1    ) { word = word>>1; }
          else if((word & 0x3   ) == 0x2    ) { word = word>>2; }
          else if((word & 0x7   ) == 0x4    ) { word = word>>3; }
          else if((word & 0xF   ) == 0x8    ) { word = word>>4; }
          else if((word & 0x1F  ) == 0x10   ) { word = word>>5; }
          else if((word & 0x3F  ) == 0x20   ) { word = word>>6; }
          else if((word & 0x7f  ) == 0x40   ) { word = word>>7; }
          else if((word & 0xff  ) == 0x80   ) { word = word>>8; }
          else if((word & 0x1ff ) == 0x100  ) { word = word>>9; }
          else if((word & 0x3ff ) == 0x200  ) { word = word>>10; }
          else if((word & 0x7ff ) == 0x400  ) { word = word>>11; }
          else if((word & 0xfff ) == 0x800  ) { word = word>>12; }
          else if((word & 0x1fff) == 0x1000 ) { word = word>>13; }
          else if((word & 0x3fff) == 0x2000 ) { word = word>>14; }
          else if((word & 0x7fff) == 0x4000 ) { word = word>>15; }

          while(word!=0) {
            if     ((word & 0x1)  == 0x1  ) {                            uncompressed.push_back((T)(last_uncompressed_word)); word = word>>1; }
            else if((word & 0x3)  == 0x2  ) { last_uncompressed_word-=1; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>2; }
            else if((word & 0x7)  == 0x4  ) { last_uncompressed_word+=1; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>3; }
            else if((word & 0xF)  == 0x8  ) { last_uncompressed_word-=2; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>4; }
            else if((word & 0x1F) == 0x10 ) { last_uncompressed_word+=2; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>5; }
            else if((word & 0x3F) == 0x20 ) { last_uncompressed_word-=3; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>6; }
            else if((word & 0x7F) == 0x40 ) { last_uncompressed_word+=3; uncompressed.push_back((T)(last_uncompressed_word)); word = word>>7; }
            else {
              // std::cout << "Huffman decompress unrecoginized bit pattern " << (std::bitset<16>) word << std::endl;
              // std::cout << "\n----\n";
              // std::cout << debugInfo();
              std::stringstream ss;
              ss << "Huffman decompress unrecoginized bit pattern:" << hex(4,*it) << " on word number " << (it-raw.begin());
                throw datatypes_exception(ss.str());
            }
          }
      
        } //end else huffman bit on
      }//end for loop over data words
  
      // uncompressed.resize(outpos);
      
    }

};




}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
