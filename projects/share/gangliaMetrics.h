#ifndef _UBOONE_DATATYPES_HASGANGLIAMETRICS_H
#define _UBOONE_DATATYPES_HASGANGLIAMETRICS_H  1

#include <map>
#include <memory>
#include <iostream>
#include <mutex>
#include <vector>
#include <cassert>
#include <stdexcept>        

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
namespace ganglia {
using namespace gov::fnal::uboone;


template<typename T>struct RATE {
    typedef T type;
    typedef double preferred_type;
};
template<typename T>struct AVERAGE {
    typedef T type;
    typedef double preferred_type;
};
template<typename T>struct COUNTER {
    typedef T type;
    typedef uint32_t preferred_type;
};
template<typename T>struct VALUE {
    typedef T type;
    typedef double preferred_type;
};
template<typename T>struct MINIMUM {
    typedef T type;
    typedef uint32_t preferred_type;
};
template<typename T>struct MAXIMUM {
    typedef T type;
    typedef uint32_t preferred_type;
};
template<typename T>struct STDEV {
    typedef T type;
    typedef double preferred_type;
};
template<typename T>struct HEARTBEAT {
    typedef T type;
    typedef uint32_t preferred_type;
};

template<template<typename> class, typename>
bool enable_metricts(bool flag=true) noexcept {
    static bool _flag(flag);
    return _flag;
}

namespace detail {
template <template<typename> class Q, typename T, bool>
struct quantity_impl {
    quantity_impl(std::string const& ,std::string const& ) {
        if(enable_metricts<Q,T>())
            std::cout << "++ default quantity_impl::quantity_impl this="<<std::hex<< this << "\n";
    }

    void publish(typename Q<T>::type const& ) noexcept {
        if(enable_metricts<Q,T>())
            std::cout << "++ default quantity_impl::publish this="<<std::hex << this <<"\n";
    }
};

struct destructable {
    virtual void destruct() noexcept=0;
};

class MetricDestructor {
public:
    static MetricDestructor& instance() {
        static MetricDestructor _self;
        return _self;
    }

    void addMetric(detail::destructable* destructable) {
        assert(destructable!=nullptr);

        if(destructable==nullptr)
            throw std::invalid_argument("destructable is nullptr");

        std::lock_guard<std::mutex> guard(_metrics_lock);
        _metrics.push_back(destructable);
    }

    void destuctMetrics() {
        std::lock_guard<std::mutex> guard(_metrics_lock);
        for(auto p:_metrics) {
            assert(p!=nullptr);
            if(p==nullptr)
                throw std::logic_error("destructable is nullptr");
            p->destruct();
        }
    }


    MetricDestructor ( MetricDestructor const& ) = delete;
    MetricDestructor& operator= ( MetricDestructor const& ) = delete;
    MetricDestructor ( MetricDestructor && ) = delete;
    MetricDestructor& operator= ( MetricDestructor && ) = delete;

private:
    MetricDestructor():_metrics_lock {},_metrics {} {
        _metrics.reserve(512);
    }

private:
    std::mutex  _metrics_lock;
    std::vector<destructable*> _metrics;
};

} // end of namespace detail

template<template<typename> class Q, typename T = typename Q<void>::preferred_type>
class Metric final: public detail::destructable {
    struct quantity : detail::quantity_impl <Q,T, true> {
        explicit quantity(std::string const& name, std::string const& unit)
            :detail::quantity_impl <Q,T,true>(name,unit) {
        }

        void publish(typename Q<T>::type const& value) noexcept {
            return detail::quantity_impl <Q,T,true>::publish(value);
        }
    };

    using quantity_ptr= std::shared_ptr<quantity>;
    typedef std::map<std::string,quantity_ptr> metrics_map_t;

    friend class MetricDestructor;
public:
    static quantity_ptr named(std::string const& name, std::string const& units) noexcept {
        return self().find_create(name,units);
    }


    Metric ( Metric const& ) = delete;
    Metric& operator= ( Metric const& ) = delete;
    Metric ( Metric && ) = delete;
    Metric& operator= ( Metric && ) = delete;

private:
    static Metric& self() {
        static Metric _self;
        return _self;
    }

    Metric():_metrics_lock {},_metrics {} {
        detail::MetricDestructor::instance().addMetric(this);
    }

    quantity_ptr find_create(std::string const& name, std::string const& units) noexcept {
        std::lock_guard<std::mutex> guard(_metrics_lock);
        auto& retVal= _metrics[name];
        if(!retVal)
            retVal.reset(new quantity{name,units});

        return retVal;
    }

    void destruct() noexcept override {
        std::lock_guard<std::mutex> guard(_metrics_lock);
        _metrics.clear();
    }

private:
    std::mutex _metrics_lock;
    metrics_map_t _metrics;
};



}  // end of namespace ganglia
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif //_UBOONE_DATATYPES_HASGANGLIAMETRICS_H

// kate: indent-mode cstyle; replace-tabs on; 


