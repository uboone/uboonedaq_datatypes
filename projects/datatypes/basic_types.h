#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H 1
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
  ub_RawDataBlock::const_iterator& begin() { return _begin; }
  ub_RawDataBlock::const_iterator& end()   { return _end; }
  
 private:    
  ub_RawDataBlock::const_iterator _begin;
  ub_RawDataBlock::const_iterator _end;
  
};

typedef std::vector ub_RawDataContainer_t;
typedef ub_RawDataBlock< ub_RawDataContainter_t,ub_RawDataWord_t> ub_RawData_t;


template <typename HEADER, typename TRAILER> 
class ub_MarkedRawData
{
 public:
  enum class data_types : size_t {rawdata, data, header,  trailer};
  
  HEADER  const& header()  const {return * (reinterpret_cast<HEADER  const *>( std::get<data_types::header >(_data_markers).get()) );}
  TRAILER const& trailer() const {return * (reinterpret_cast<TRAILER const *>( std::get<data_types::trailer>(_data_markers).get()) );}    

  ub_RawData_t const& data() const {return std::get<data_types::data>(_data_markers);}
  
 private:    
  std::tuple<ub_RawData_T,ub_RawData_t,ub_RawData_t::const_iterator,ub_RawData_t::const_iterator> _data_markers;
};
 

}
}
}
}

#endif /* #ifdef BASIC_TYPES_H */
