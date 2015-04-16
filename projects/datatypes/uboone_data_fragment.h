#ifndef _UBOONE_DATATYPES_DATAFRAGMENT_H
#define _UBOONE_DATATYPES_DATAFRAGMENT_H  1

#include <fstream>
#include <vector>
#include <openssl/md5.h>
#include <algorithm>
#include <iterator>

# define DO_STDVEC 1


typedef uint16_t raw_data_type;

template<typename TYPE, template<typename,typename> class CONTAINER,  typename ALLOC> 
class data_containter_wrap: public CONTAINER<TYPE,ALLOC> {
public:
explicit data_containter_wrap():CONTAINER<TYPE,ALLOC>(0){}
explicit data_containter_wrap(std::size_t size):CONTAINER<TYPE,ALLOC>(size){}
explicit data_containter_wrap(std::size_t size, const TYPE& value):CONTAINER<TYPE,ALLOC>(size,value){}
};

#if DO_STDVEC == 1
template <class TYPE,class ALLOC = std::allocator<TYPE>> using raw_data_containter = data_containter_wrap<TYPE, std::vector, ALLOC>;
#else
#include "share/QuickVec.hh"
template <class TYPE,class ALLOC = std::allocator<TYPE>> using raw_data_containter = data_containter_wrap<TYPE, QuickVec, ALLOC>;
#endif



namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

constexpr uint32_t EVENTTRAILER{0xe0000000};
constexpr uint32_t EVENTHEADER{0xffffffff};

constexpr uint32_t UBOONE_EHDR{0xE974E974};
constexpr uint32_t UBOONE_ETLR{0x974E974E};



typedef raw_data_containter<raw_data_type> raw_fragment_t;

class datatypes_exception : public std::exception
{
public:
    datatypes_exception(std::string const& message, std::string const& name="datatypes_exception");
    virtual const char *    what () const noexcept;
    virtual ~datatypes_exception() noexcept;
private:
    std::string _name;
    std::string _message;
};

struct artdaq_fragment_header final{
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


    template<typename T>
    constexpr static std::size_t bytes_to_pad() {        
        return  sizeof(T) % sizeof(artdaq_fragment_header::RawDataType);
    }

    template<typename T>
    constexpr static std::size_t padding_bytes_count() {        
        return (bytes_to_pad<T>() >0) ? (sizeof(artdaq_fragment_header::RawDataType) - bytes_to_pad<T>()) : 0;
    }
    
    template<typename T>
    constexpr static std::size_t padded_size_of() {
        return sizeof(T) + padding_bytes_count<T>();
    }

    template<typename T> 
    constexpr static std::size_t padded_wordcount_of() {
        return sizeof(T)/sizeof(artdaq_fragment_header::RawDataType) + (bytes_to_pad<T>() >0 ? 1 : 0);
    }    
};

static_assert((artdaq_fragment_header::bytes_to_pad<artdaq_fragment_header>() == 0),
              "sizeof(RawFragmentHeader) is not an integer multiple of sizeof(artdaq_fragment_header::RawDataType)!");

struct ub_fragment_header final
{
    //do not reorder or change this data structure
    std::size_t   total_fragment_wordcount;           //1st position represents a total size of crate data
    uint8_t  	  fragment_format_version;            //2nd position represents a data format version
    bool  	  is_fragment_complete;               //3rd position 1 is complete; 0 is incomplete
    std::size_t   raw_fragment_wordcount;             //4th position size of raw dma'ed data, includeing all headers/trailers
    std::size_t   raw_fragment_beginning_word_offset; //5th position offest to the begining of the raw dma'ed data
    unsigned char md5hash[MD5_DIGEST_LENGTH];    //6th position md5 hash of raw dma'ed data, which was calculated by seb
    uint32_t      reserved[4];                        //7th position reserved

    ub_fragment_header():
        total_fragment_wordcount {0},
        fragment_format_version {0},
        is_fragment_complete {0},
        raw_fragment_wordcount {0},
	raw_fragment_beginning_word_offset {0},
	reserved {1,2,3,4} {}

    void calculateMD5hash(unsigned char const* addr, std::size_t bytes) noexcept;
    
    bool verifyMD5hash(unsigned char const* addr, std::size_t bytes) const noexcept;

    bool compare(ub_fragment_header const&, bool do_rethrow=false) const throw(datatypes_exception);

};
constexpr std::size_t ub_fragment_header_size = sizeof(ub_fragment_header);
constexpr std::size_t ub_fragment_header_wordcount = sizeof(ub_fragment_header)/sizeof(artdaq_fragment_header::RawDataType);

static_assert( (artdaq_fragment_header::bytes_to_pad<ub_fragment_header>() == 0),
    "sizeof(ub_fragment_header) is not an integer multiple of sizeof(artdaq_fragment_header::RawDataType)!");


struct ub_event_header final
{
    //do not reorder or change this data structure
    uint32_t 	mark_E974;                         	//always E974
    std::size_t total_event_wordcount;                  //1st position represents a total size of event including header/trailer words
    uint8_t     event_format_version;                   //2nd position represents a data format version
    bool        is_event_complete;                      //3rd position 1 is complete; 0 is incomplete cumulative state of all fragments
    std::size_t event_fragment_count;                   //4rd position represent number of events per fragment
    
    std::size_t raw_event_fragments_wordcount;          //5th position size of raw dma'ed data, includeing all headers/trailers
    std::size_t event_global_header_word_offset; 	//6th position offest to the begining of the global header offest
    uint32_t 	reserved[4];                            //7th position reserved

    ub_event_header():   mark_E974{UBOONE_EHDR},
			 total_event_wordcount {0},
                         event_format_version {0},
                         is_event_complete {0},
                         event_fragment_count{0},
                         raw_event_fragments_wordcount {0},
    event_global_header_word_offset {0},
    reserved {1,2,3,4} {}

    //bool compare(ub_event_header const&, bool do_rethrow=false) const throw(datatypes_exception){return true;};

};
constexpr std::size_t ub_event_header_size = sizeof(ub_event_header);
constexpr std::size_t ub_event_header_wordcount = sizeof(ub_event_header)/sizeof(artdaq_fragment_header::RawDataType);

static_assert( (artdaq_fragment_header::bytes_to_pad<ub_event_header>() == 0),
    "sizeof(ub_event_header) is not an integer multiple of sizeofartdaq_fragment_header::RawDataType)!");

    

struct ub_event_trailer final
{
    //do not reorder or change this data structure
    uint32_t reserved[4];                       //1st position reserved
    uint32_t mark_974E;                      	//always E974
    ub_event_trailer():reserved {1,2,3,4},mark_974E{UBOONE_ETLR}{}
};
constexpr std::size_t ub_event_trailer_size = sizeof(ub_event_trailer);
constexpr std::size_t ub_event_trailer_wordcount = sizeof(ub_event_trailer)/sizeof(artdaq_fragment_header::RawDataType);

static_assert( (artdaq_fragment_header::bytes_to_pad<ub_event_trailer>() == 0),
    "sizeof(ub_event_trailer) is not an integer multiple of sizeof(artdaq_fragment_header::RawDataType)!");
    
constexpr std::size_t ub_size_t_wordcount = artdaq_fragment_header::padded_wordcount_of<std::size_t>();
static_assert( (artdaq_fragment_header::bytes_to_pad<std::size_t>() == 0),
    "sizeof(std::size_t) is not an integer multiple of sizeof(artdaq_fragment_header::RawDataType)!");

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATATYPES_DATAFRAGMENT_H

