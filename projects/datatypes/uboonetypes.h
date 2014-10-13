#ifndef _UBOONE_TYPES_H
#define _UBOONE_TYPES_H 1
#include <inttypes.h>
#include <tuple.h>

typedef uint16_t ub_RawDataWord_t;
typedef std::vector<ub_RawDataWord_t> ub_RawEvent_t;

typedef uint32_t ub_VersionWord_t;
typedef uint32_t ub_SizeWord_t;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template< template<class,class> class CONTAINER, typename TYPE>
class ub_RawDataBlock{

 public:  
  typedef typename CONTAINER<TYPE, std::allocator<TYPE>>::const_iterator const_iterator;

 ub_RawDataBlock(ub_RawDataBlock::const_iterator b, ub_RawDataBlock::const_iterator e):
  _begin(b), _end(e) {}

  ub_RawDataBlock::const_iterator& begin() { return _begin; }
  ub_RawDataBlock::const_iterator& end()   { return _end; }

  size_t typeSize() { return sizeof(TYPE); }
  
 private:    
  ub_RawDataBlock::const_iterator _begin;
  ub_RawDataBlock::const_iterator _end;
  
};

typedef std::vector ub_RawDataContainer_t;
typedef ub_RawDataBlock< ub_RawDataContainter_t,ub_RawDataWord_t> ub_RawData_t;


template <typename HEADER, typename TRAILER> 
class ub_MarkedRawDataBlock
{
 public:
  enum class data_types : size_t {rawdata, data, header,  trailer};
  
 ub_MarkedRawDataBlock(ub_RawData_t r, ub_RawData_t d, ub_RawData_t::const_iterator h, ub_RawData_t::const_iterator t):
  _data_markers(std::make_tuple(r,d,h,t)){}
  
  HEADER  const& header()  const {return * (reinterpret_cast<HEADER  const *>( std::get<data_types::header >(_data_markers).get()) );}
  TRAILER const& trailer() const {return * (reinterpret_cast<TRAILER const *>( std::get<data_types::trailer>(_data_markers).get()) );}    
  
  ub_RawData_t const& data()    const {return std::get<data_types::data   >(_data_markers);}
  ub_RawData_t const& rawdata() const {return std::get<data_types::rawdata>(_data_markers);}
  
 private:    
  std::tuple<ub_RawData_t,ub_RawData_t,ub_RawData_t::const_iterator,ub_RawData_t::const_iterator> _data_markers;
};
 
class ub_MarkedRawDataBlockCreator
{
 public:
  template <typename HEADER, typename TRAILER> 
    ub_MarkedRawDataBlock<HEADER,TRAILER>* 
    createMarkedRawDataBlock(ub_RawData_t r, ub_RawData_t d, ub_RawData_t::const_iterator h, ub_RawData_t::const_iterator t)
    { return std::unique_ptr< ub_MarkedRawDataBlock<HEADER,TRAILER> >(new ub_MarkedRawDataBlock<HEADER,TRAILER>(r,d,ub,t));}
}

/* The gps structure is what is read off the card
 * in the trigger crate and represents the GPS time at the arrival of a PPS.
 */
typedef struct gps {
    uint32_t lower;
    uint32_t upper;
} gps_t;  /* 8 bytes */


/* Not very well-structured, but the bits are all scrambled so this is actually easiest to manipulate.
 * See the datatypes code for unpacking. --NJT
 */
#define N_ACTIVITY_HIST 4
typedef struct trigger_data {
  uint16_t    word1; /* holds sample number, reminder, busy flag      */
  uint16_t    word2; /* holds frame number (low bits)                 */
  uint16_t    word3; /* hold trigger number, frame number (high bits) */
  uint16_t    word4; /* holds trigger number (high bits)              */
  uint16_t    word5; /* holds trigger bits                            */
  uint16_t    word6; /* bit0 is phase[1], rest is unusued             */
  uint16_t    word7; /* unused, should be 0xffff                      */
  uint16_t    word8; /* unused, should be 0xfff                       */
} trigger_data_t;  /* total 16 bytes */

/* Empty header and trailer options. Likely never need an empty header, but will have empty trailers for FEMs.
 *
 */
typedef struct empty_header
{} empty_header_t;
typedef struct empty_trailer
{} empty_trailer_t;

/**
   This replaces tmub ../gps/symm.h
 **/
typedef struct gps_time 
{
  // 2^32 = 4.E9 . Thus 32 bits allows for both (2013-1970)*3.14e7 seconds and 
  // enough nanoseconds to span a second.
  uint32_t second; // seconds since the epoch. 
  uint32_t micro;  // microseconds since the second. 
  uint32_t nano;  // nanoseconds since the second. 
  gps_time(){};
} gps_time_t;


/**PMT Card/Module Header format: (each module sends a header followed by data)**/
typedef struct pmt_card_header 
{
  uint32_t id_and_module; 
  uint32_t word_count;  //this is number of 16-bit words.
  uint32_t event_number;  
  uint32_t frame_number;  
  uint32_t checksum;
  uint32_t trig_frame_and_sample;  
} pmt_card_header_t;


/** PMT Control Words. These should be at begin and end of each block of pmt data.
Header word format: (should be 0x4000)
Trailer word format: (should be 0xc000)
**/
typedef struct pmt_data_header
{
  uint16_t header;
}pmt_data_header_t;

typedef struct pmt_data_trailer
{
  uint16_t trailer;
}pmt_data_trailer_t;

typedef struct pmt_window_header 
{
  uint16_t ch_and_disc; 
  uint16_t frame_and_sample1; //bits 6-8: last 3 readout frame bits; bits 1-5: upper 5 bits of readout sample
  uint16_t sample2; //lower 12 bits of readout sample
} pmt_window_header_t;

}
}
}
}

#endif /* #ifdef BASIC_TYPES_H */
