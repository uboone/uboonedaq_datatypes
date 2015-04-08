#ifndef _UBOONETYPES_BEAMDATA_H
#define _UBOONETYPES_BEAMDATA_H

#include <iostream>
#include <vector>
#include <iomanip>


#include "boostSerialization.h"
#include "constants.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

class ub_BeamData {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        if(version > 0)
            ar & device_name & units & seconds & milli_seconds & device_data;
    }

public:
    ub_BeamData();

    std::string getDeviceName() const noexcept;
    std::string getUnits() const noexcept;
    uint32_t getSeconds() const noexcept;
    uint16_t getMilliSeconds() const noexcept;
    std::vector<double> const& getData() const noexcept;

    void setDeviceName(std::string const& val) noexcept;
    void setUnits(std::string const& val) noexcept;
    void setSeconds(uint32_t const& val) noexcept;
    void setMilliSeconds(uint16_t const& val) noexcept;
    void setData(std::vector<double> const& val) noexcept;
    void pushData(double const& val) noexcept;

private:
    std::string device_name;
    std::string units;
    uint32_t seconds; // GPS clock. Since Jan 1, 2012.
    uint16_t milli_seconds;
    std::vector<double> device_data;
};
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamData &bd);

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_BeamData, gov::fnal::uboone::datatypes::constants::VERSION)

#endif /* #ifndef BOONETYPES_H */
