#ifndef _UBOONE_DATATYPES_DATAFRAGMENT_H
#define _UBOONE_DATATYPES_DATAFRAGMENT_H  1

#include <fstream>
#include <vector>
#include <openssl/md5.h>
#include <algorithm> 
#include <iterator>

#define EVENTTRAILER 0xe0000000
#define EVENTHEADER  0xffffffff

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


#if 0
  #include "DAQdata/Fragment.hh" 
  typedef artdaq::RawDataType raw_data_type;
  typedef artdaq::Fragment    raw_data_containter;
#else
  typedef uint16_t raw_data_type;
  template <class T,class Alloc = std::allocator<T>> using raw_data_containter = std::vector<T, Alloc>;
#endif


struct artdaq_fragment_header {
  typedef  raw_data_type RawDataType;
  typedef uint64_t RawMetaDataType;
  RawMetaDataType word_count          : 32; // number of RawDataTypes in this Fragment
  RawMetaDataType version             : 16;
  RawMetaDataType type                :  8;
  RawMetaDataType metadata_word_count :  8;
  RawMetaDataType sequence_id : 48;
  RawMetaDataType fragment_id : 16;
  RawMetaDataType unused1     : 16;
  RawMetaDataType unused2     : 16;
  RawMetaDataType unused3     : 16;
  RawMetaDataType unused4     : 16;
  constexpr static std::size_t num_words() {
    return sizeof(artdaq_fragment_header) / sizeof(artdaq_fragment_header::RawDataType);    
    }
    
  template<typename T> static std::size_t padded_size_of(){
  std::size_t bytes{sizeof(T)};
  std::size_t remainder{bytes % sizeof(artdaq_fragment_header::RawDataType)};  
  return bytes + (remainder>0?sizeof(artdaq_fragment_header::RawDataType) - remainder:0);}
  
};

static_assert((artdaq_fragment_header::num_words() * sizeof(artdaq_fragment_header::RawDataType)) == sizeof(artdaq_fragment_header),
              "sizeof(RawFragmentHeader) is not an integer ""multiple of sizeof(RawDataType)!");
              
struct ub_fragment_header
{
  //do not reorder or chnge this data structure
  uint32_t total_fragment_data_size;           //1st position represents a total size of crate data
  uint8_t  fragment_format_version;            //2nd position represents a data format version 
  uint8_t  is_fragment_complete;               //3rd position 1 is complete; 0 is incomplete  
  uint32_t raw_fragment_byte_size;             //4th position size of raw dma'ed data, includeing all headers/trailers
  uint32_t raw_fragment_beginning_byte_offset; //5th position offest to the begining of the raw dma'ed data
  unsigned char md5hash[MD5_DIGEST_LENGTH];     //6th position md5 hash of raw dma'ed data, which was calculated by deb
  uint32_t reserved[4];                        //7th position reserved
  
  ub_fragment_header():
  total_fragment_data_size{0},
  fragment_format_version{0},
  is_fragment_complete{0},
  raw_fragment_byte_size{0},
  raw_fragment_beginning_byte_offset{0}{}
  
  void calculateMD5hash(raw_data_containter<raw_data_type> const& data) {
    MD5((unsigned char const*) data.data(),  data.size()*sizeof(raw_data_type) , md5hash);
  }
  
  bool verifyMD5hash(raw_data_containter<raw_data_type> const& data) {
    unsigned char md5hash_[MD5_DIGEST_LENGTH];    
    MD5((unsigned char const*) data.data(),  data.size()*sizeof(raw_data_type) , md5hash_);    
    return std::equal(std::begin(md5hash), std::end(md5hash), std::begin(md5hash_));
  }
};

constexpr std::size_t ub_fragment_header_size = sizeof(ub_fragment_header);


class datatypes_exception : public std::exception
{
public:
    datatypes_exception(std::string const& message, std::string const& name="datatypes_exception");
    virtual const char *    what () const throw ();
    virtual ~datatypes_exception() throw();
private:
    std::string _name;
    std::string _message;
};
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATATYPES_DATAFRAGMENT_H

