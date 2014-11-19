#ifndef _UBOONE_DATAUTILS_H
#define _UBOONE_DATAUTILS_H  1

#include <memory>
#include <string>
#include <cxxabi.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream> 
#include <iomanip>

#include "uboone_data_fragment.h"

namespace ub_data_types =gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template< template<class, class> class CONTAINER, typename TYPE> class ub_RawDataBlock {
public:  
    typedef typename CONTAINER<TYPE, std::allocator<TYPE>>::const_iterator const_iterator;
	
	explicit ub_RawDataBlock(ub_RawDataBlock::const_iterator begin, 
							ub_RawDataBlock::const_iterator end)
		:_begin{begin},_end{end} {}
	
    ub_RawDataBlock::const_iterator begin() const{return _begin;}
    ub_RawDataBlock::const_iterator end() const{return _end;}
	
	size_t size() const { std::distance(begin(), end());}
private:    
	ub_RawDataBlock::const_iterator _begin;
    ub_RawDataBlock::const_iterator _end;
};


typedef ub_RawDataBlock< raw_data_containter,raw_data_type> ub_RawData;
		
#define STRTAB "            "

std::string demangle(const std::type_info  &type_info);
std::string debugInfo(ub_RawData const& data);
std::string debugInfoRawData(raw_data_containter<raw_data_type>const& data);


template <typename T>
constexpr typename std::underlying_type<T>::type cast_enum(T value) {
    return static_cast<typename std::underlying_type<T>::type>(value);
};

template <typename TYPE> 
TYPE const& quick_cast(ub_RawData::const_iterator data){
	return *reinterpret_cast<TYPE const *>(&*data);
};




template <typename T, typename ... ARGS>
std::unique_ptr<T> factory(ARGS&&... args){
    return std::unique_ptr<T>(new T { std::forward<ARGS>(args)... });
};

template <typename T> 
constexpr size_t size_of(){
	return sizeof(T)/sizeof(raw_data_type);
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATAUTILS_H

