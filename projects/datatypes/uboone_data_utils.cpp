#include <memory>
#include <string>
#include <cxxabi.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include  <iomanip>

#include "uboone_data_utils.h"

using namespace ub_data_types;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

std::string demangle(const std::type_info  &type_info) noexcept
{
    int     status;
    char   *name = abi::__cxa_demangle(type_info.name(), 0, 0, &status);
    std::string retValue {name};
    free(name);
    return retValue.empty()?type_info.name():retValue;
}

std::string debugInfo(ub_RawData const& data) noexcept
{
    std::ostringstream os;
    os <<"Buffer size is "<< std::dec << std::distance(data.begin(), data.end())  *sizeof(raw_data_type);
    os << " bytes, or "<< std::distance(data.begin(), data.end()) << " elements " ;
    os << sizeof(raw_data_type) << " bytes each."<< std::endl <<  STRTAB;

    size_t counter {0};

    std::for_each(data.begin(), data.end(),
    [&os,&counter](raw_data_type data) {
        os << std::hex <<std::setfill('0') << std::setw(4) << data ;
        if (++counter%0x10 == 0)
        {
            os <<std::endl  << STRTAB;

            if (counter%0x100 == 0)
                os <<std::endl  << STRTAB;
        }
        else
            os << " ";
    }
                 );

    os  <<std::endl;

    return os.str();
}

std::string debugInfoShort(ub_RawData const& data) noexcept
{
    std::ostringstream os;
    
    if(data.size() >= 0x100)
    {
        os <<"Total Buffer size is "<< std::dec << std::distance(data.begin(), data.end())  *sizeof(raw_data_type);
        os << " bytes, or "<< std::distance(data.begin(), data.end()) << " elements " ;
        os << sizeof(raw_data_type) << " bytes each."<< std::endl;
        os << STRTAB<< "Beginning data block --------------------------------------------------\n";
        os << STRTAB<< debugInfo(ub_RawData(data.begin(),data.begin()+0x80));
        os << STRTAB<< "Ending data block -----------------------------------------------------\n";
        os << STRTAB<< debugInfo(ub_RawData(data.begin()+data.size()-0x80,data.end()));
    }
    else
       debugInfo(data);

    os  <<std::endl;

    return os.str();
}
std::string debugInfoRawData(raw_data_container<raw_data_type>const& data) noexcept
{
    return debugInfo(ub_RawData(data.begin(),data.end()));
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov
