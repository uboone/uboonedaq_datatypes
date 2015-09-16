#ifndef _UBOONE_DATATYPES_HASGANGLIAMETRICS_H
#define _UBOONE_DATATYPES_HASGANGLIAMETRICS_H  1

#include <map>
#include <memory>
#include <iostream> 

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
namespace ganglia {
using namespace gov::fnal::uboone;


template<typename T>struct RATE {typedef T type; typedef double preferred_type;};    
template<typename T>struct AVERAGE {typedef T type; typedef double preferred_type;};
template<typename T>struct COUNTER {typedef T type; typedef uint32_t preferred_type;};
template<typename T>struct VALUE {typedef T type; typedef double preferred_type;};
template<typename T>struct MINIMUM {typedef T type; typedef uint32_t preferred_type;};
template<typename T>struct MAXIMUM {typedef T type;typedef uint32_t preferred_type;};
template<typename T>struct STDEV {typedef T type; typedef uint32_t preferred_type;};
template<typename T>struct HEARTBEAT {typedef T type;typedef uint32_t preferred_type;};

template<template<typename> class, typename T>
bool enable_metricts(bool flag=false) noexcept{
  static bool _flag(flag);
  return _flag;
}

namespace detail {     
  template <typename T, template<typename> class Q, bool>
  struct quantity_impl {
    quantity_impl(std::string const& ,std::string const& ){
      if(enable_metricts<Q,T>()) 
        std::cout << "++ default quantity_impl::quantity_impl this="<<std::hex<< this << "\n";
    }
    
    void publish(typename Q<T>::type const& ) noexcept {
      if(enable_metricts<Q,T>())       
        std::cout << "++ default quantity_impl::publish this="<<std::hex << this <<"\n";      
    }
  };
}

template<template<typename> class Q, typename T>
class Metric {    
    struct quantity : detail::quantity_impl <T,Q, true> {
        explicit quantity(std::string const& name, std::string const& unit)
          :detail::quantity_impl <T,Q,true>(name,unit){          
        }
        
        void publish(typename Q<T>::type const& value) noexcept { 
          return detail::quantity_impl <T,Q,true>::publish(value);
        }
    };
    
    using quantity_ptr= std::shared_ptr<quantity>;
    typedef std::map<std::string,quantity_ptr> metrics_map_t;
    
public:
    static quantity_ptr named(std::string const& name, std::string const& unit) noexcept {
        auto& retVal= metrics[name];                
        if(!retVal)
          retVal.reset(new quantity{name,unit});

        return retVal;
    }
    
private:    
   static metrics_map_t metrics;
};
    
template<template<typename> class Q, typename T>
  typename Metric<Q,T>::metrics_map_t Metric<Q,T>::metrics;

}  // end of namespace ganglia 
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATATYPES_HASGANGLIAMETRICS_H

// kate: indent-mode cstyle; replace-tabs on; 


