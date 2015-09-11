#ifndef _UBOONE_DATATYPES_HASGANGLIAMETRICS_H
#define _UBOONE_DATATYPES_HASGANGLIAMETRICS_H  1

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<typename T>struct RATE {typedef T type;};    
template<typename T>struct AVERAGE {typedef T type;};
template<typename T>struct COUNTER {typedef T type;};

template<template<typename> class Q>
class GangliaMetric {
    
    template<typename T>
    struct quantity {
        explicit quantity(std::string const & name, std::string const& description):_name(name),_description(description) {}
    
        void publish(typename Q<T>::type ) noexcept {}

        std::string _name;
        std::string _description;
    };
    
public:
template<typename T>    
    static  quantity<T> named(std::string const &name, std::string const & description="") noexcept {
        return quantity<T>(name,description);
    }

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATATYPES_HASGANGLIAMETRICS_H

// kate: indent-mode cstyle; replace-tabs on; 



