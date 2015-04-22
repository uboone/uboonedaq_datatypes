#ifndef _UBOONE_DATAUTILS_H
#define _UBOONE_DATAUTILS_H  1

#include <memory>
#include <string>
#include <cassert>
#include <cxxabi.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "uboone_data_fragment.h"

#define UNUSED(x) (void)(x)

namespace ub_data_types =gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template< template<class, class> class CONTAINER, typename TYPE> class ub_RawDataBlock final{
public:
    typedef typename CONTAINER<TYPE, std::allocator<TYPE>>::const_iterator const_iterator;

    explicit ub_RawDataBlock(ub_RawDataBlock::const_iterator begin,
                             ub_RawDataBlock::const_iterator end)
        :_begin {begin},_end {end} {}

    ub_RawDataBlock::const_iterator begin() const noexcept{
        return _begin;
    }
    ub_RawDataBlock::const_iterator end() const  noexcept{
        return _end;
    }

    size_t size() const {
        return std::distance(begin(), end());
    }
    bool compare(ub_RawDataBlock const&,bool do_rethrow=false) const throw(datatypes_exception);
    
private:
    ub_RawDataBlock::const_iterator _begin;
    ub_RawDataBlock::const_iterator _end;
};

template< template<class, class> class CONTAINER, typename TYPE>
bool ub_RawDataBlock< CONTAINER,  TYPE>::compare(ub_RawDataBlock const& data_block,bool do_rethrow) const throw(datatypes_exception)
{
    try {
        if(size()!=data_block.size())
            throw datatypes_exception("Error: ub_RawDataBlock has a different size.");

        if (! std::equal (begin(), end(), data_block.begin()) )
            throw datatypes_exception("Error: ub_RawDataBlock point to different data.");
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_RawDataBlock< CONTAINER,  TYPE>::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_RawDataBlock< CONTAINER,  TYPE>::compare()");
        else
            return false;
    }
    return true;

}

typedef ub_RawDataBlock< raw_data_containter,raw_data_type> ub_RawData;

#define STRTAB "            "

std::string demangle(const std::type_info  &type_info) noexcept;
std::string debugInfo(ub_RawData const& data) noexcept;
std::string debugInfoShort(ub_RawData const& data) noexcept;

std::string debugInfoRawData(raw_data_containter<raw_data_type>const& data) noexcept;


template <typename T>
constexpr typename std::underlying_type<T>::type cast_enum(T value) {
    return static_cast<typename std::underlying_type<T>::type>(value);
}

template <typename TYPE>
TYPE const& quick_cast(ub_RawData::const_iterator data) {
    return *reinterpret_cast<TYPE const *>(&*data);
}




template <typename T, typename ... ARGS>
std::unique_ptr<T> factory(ARGS&&... args) {
    return std::unique_ptr<T>(new T { std::forward<ARGS>(args)... });
}

template <typename T>
constexpr size_t size_of() {
    return sizeof(T)/sizeof(raw_data_type);
}

template <typename T>
std::string make_compare_message(std::string name, std::string what,  T const& left, T const& right)
{
    std::ostringstream os;
    os << "Error: "<< name << " has different " << what;
    os << "; <this,comparable>=< ";
    os << left << "," << right << ">";
    return os.str();
}

template <typename T>
std::string to_hex_string(T& value)
{
    std::ostringstream os;
    os <<"0x" << std::hex << value;
    return os.str();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATAUTILS_H


