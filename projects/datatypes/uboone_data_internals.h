#ifndef _UBOONE_DATATYPES_DATAINTERNALS_H
#define _UBOONE_DATATYPES_DATAINTERNALS_H  1

#include <tuple>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <cxxabi.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include  <iomanip>

#include "uboone_data_fragment.h"
#include "uboone_data_utils.h"
#include "empty_trailer.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename HEADER, typename TRAILER> class ub_MarkedRawDataBlock
{
public:
    typedef  HEADER header_type;
    typedef  TRAILER trailer_type;

    enum class data_types : size_t {
        rawdata, data, header,  trailer
    };

    static_assert (sizeof(HEADER)%2==0 ,   "CODING BUG: HEADER must have even byte count, fix C++ header files.");
    static_assert (sizeof(TRAILER)%2 == 0, "CODING BUG: TRAILER must have even byte count, fix C++ header files.");

    explicit ub_MarkedRawDataBlock(ub_RawData const& rawdata):ub_MarkedRawDataBlock(rawdata,NULL){}    

    HEADER const&  header() const noexcept {
        return *(reinterpret_cast<HEADER const *>(&*std::get<cast_enum(data_types::header)>(_data_markers)) );
    }
    TRAILER const&  trailer() const noexcept {
        return * (reinterpret_cast<TRAILER const *>(&*std::get<cast_enum(data_types::trailer)>(_data_markers)) );
    }
    ub_RawData const&  data() const noexcept {
        return std::get<cast_enum(data_types::data)>(_data_markers);
    }
    ub_RawData const&  rawdata() const noexcept {
        return std::get<cast_enum(data_types::rawdata)>(_data_markers);
    }

    std::string debugInfo()const noexcept;
    bool compare(ub_MarkedRawDataBlock const&,bool do_rethrow=false) const throw(datatypes_exception);

    size_t minsize()const {
        return  size_of<HEADER>()+size_of<TRAILER>();
    }

private:    
    ub_MarkedRawDataBlock(ub_RawData const& rawdata, void*)
        : _data_markers {
		std::make_tuple(rawdata,
		ub_RawData(std::next(rawdata.begin(),size_of<HEADER>()),
			  std::next(rawdata.begin(),rawdata.size()-size_of<TRAILER>())),
		rawdata.begin(),
		std::next(rawdata.begin(),rawdata.size()-size_of<TRAILER>()))
	} {};
    
private:
    std::tuple<ub_RawData,ub_RawData,ub_RawData::const_iterator,ub_RawData::const_iterator> _data_markers;    
};

template <typename HEADER, typename TRAILER>
std::string ub_MarkedRawDataBlock< HEADER,  TRAILER>::debugInfo()const noexcept
{
    std::ostringstream os;

    os << "Object " << demangle(typeid(this))<< "."<< std::endl;
    os << "  RAW Data: " << ub_data_types::debugInfo (rawdata());
    os << "  Data:     " << ub_data_types::debugInfo (data());

    return os.str();
}

template <typename HEADER, typename TRAILER>
bool ub_MarkedRawDataBlock< HEADER,  TRAILER>::compare(ub_MarkedRawDataBlock const& data_block,bool do_rethrow) const throw(datatypes_exception)
{
    try {
        if(rawdata().compare(data_block.rawdata()))
            throw datatypes_exception("ub_MarkedRawDataBlock.rawdata() have different data");
        if(data().compare(data_block.data()))
            throw datatypes_exception("ub_MarkedRawDataBlock.data() have different data");
        if(header().compare(data_block.header()))
            throw datatypes_exception("ub_MarkedRawDataBlock.header() have different data");
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception ub_MarkedRawDataBlock< HEADER,  TRAILER>::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception ub_MarkedRawDataBlock< HEADER,  TRAILER>::compare()");
        else
            return false;
    }
    return true;

}


template <typename TYPE>
typename TYPE::header_type const&  const_header_from(ub_RawData const&data) noexcept{
    return *(reinterpret_cast<typename TYPE::header_type const*>(&*
             data.begin()));
}

template <typename TYPE>
typename TYPE::trailer_type const&  const_trailer_from(ub_RawData const&data) noexcept{
    return *(reinterpret_cast<typename TYPE::trailer_type const*>(&*
             (data.begin()+data.size()-size_of<typename TYPE::trailer_type>() ) ));
}

template <typename T> 
bool invoke_isValid_member(T const&) noexcept{
    return true;
}

template<typename MRCD>
bool peek_at_next_event(bool flag=false) noexcept{
  static bool _flag(flag);
  return _flag;
}
template<typename MRCD>
bool handle_missing_words(bool flag=false) noexcept{
  static bool _flag(flag);
  return _flag;
}



}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_DATATYPES_DATAINTERNALS_H

