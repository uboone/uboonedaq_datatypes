#ifndef _UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H
#define _UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H 1
#include "uboone_data_utils.h"
#include <map>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


typedef std::size_t (* factory_constructor )(ub_RawData const rawdata, bool createHeaderFromData); 

template <typename T>
struct DissectorAdapter 
{
      static std::size_t getSizeOfDissectableCrateData(ub_RawData const rawdata, bool createHeaderFromData) {
        std::unique_ptr<T> ptr{new T {rawdata, createHeaderFromData} };        
        return ptr->getSizeOfDissectableCrateData();		
      }
};


class DissectorFactory
{
    typedef std::map<std::string,std::pair<uint8_t ,factory_constructor> > DissectorMap_t;   
public:
     DissectorFactory();
    ~DissectorFactory();

    void registerDissector(std::string const& name, uint8_t const& version, factory_constructor constructor);
    std::size_t getSizeOfDissectableCrateData(std::string const& name, uint8_t const& version, ub_RawData const rawdata, bool createHeaderFromData);

    
    DissectorFactory ( DissectorFactory const & ) = delete;
    DissectorFactory& operator= ( DissectorFactory const & ) = delete;
    DissectorFactory ( DissectorFactory const && ) = delete;
    DissectorFactory& operator= ( DissectorFactory const && ) = delete;

private:
    DissectorMap_t _dissectors;
};

extern "C"  DissectorFactory& getDissectorFactory();

    
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_DATATYPES_DATAFRAGMENT_METADATA_H

