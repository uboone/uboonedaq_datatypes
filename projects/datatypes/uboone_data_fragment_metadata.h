#ifndef _UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H
#define _UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H 1
#include "uboone_data_utils.h"

#include "ub_CrateHeader_v6.h"

#include <map>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using crate_header_t = ub_CrateHeader_v6;

typedef crate_header_t (* crate_header_builder )(ub_RawData const& rawdata, bool initializeHeaderFromRawData); 

template <typename T>
struct DissectorAdapter 
{
      static crate_header_t buildCrateHeader(ub_RawData const& rawdata, bool initializeHeaderFromRawData) {
        std::unique_ptr<T> ptr{new T {rawdata, initializeHeaderFromRawData} };
        assert(ptr);
        crate_header_t header;
        ptr->crateHeader()->copyOut(header);
        return header;
        }
};


class DissectorFactory
{
    typedef std::map<std::string,std::pair<uint8_t , crate_header_builder> > DissectorMap_t;   
public:
     DissectorFactory();
    ~DissectorFactory();

    void registerDissector(std::string const& name, uint8_t const& version, crate_header_builder builder);
    crate_header_t buildCrateHeaderFromRawData(std::string const& name, uint8_t const& version, ub_RawData const& rawdata, bool initializeHeaderFromRawData);
    
    DissectorFactory ( DissectorFactory const & ) = delete;
    DissectorFactory& operator= ( DissectorFactory const & ) = delete;
    DissectorFactory ( DissectorFactory && ) = delete;
    DissectorFactory& operator= ( DissectorFactory  && ) = delete;

private:
    DissectorMap_t _dissectors={};
};

extern "C"  DissectorFactory& getDissectorFactory();

    
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H

