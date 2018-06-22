#ifndef _UBOONETYPES_BEAMRECORD_H
#define _UBOONETYPES_BEAMRECORD_H

#include <iostream>
#include "boostSerialization.h"
#include "constants.h"
#include "ub_BeamHeader.h"
#include "ub_BeamData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone;

typedef  ub_BeamHeader ub_BeamHeader_t;
typedef  ub_BeamData ub_BeamData_t;
typedef std::vector<ub_BeamData_t> ub_BeamDataVector_t;

class ub_BeamRecord final{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        if(version >0)
            ar &  _beam_header & _beam_data_vector;
    }

public:
    ub_BeamHeader_t const& beamHeader() const noexcept;
    void setBeamHeader (ub_BeamHeader_t const& beam_header) noexcept;
    ub_BeamDataVector_t const& beamDataVector() const noexcept;
    void clearBeamDataVector() noexcept;
    void insertBeamData(ub_BeamData_t const& beam_data) noexcept;

    std::string debugInfo()const noexcept;

private:
    ub_BeamHeader_t  _beam_header;
    ub_BeamDataVector_t _beam_data_vector;
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

std::ostream & operator<<(std::ostream &os, const gov::fnal::uboone::datatypes::ub_BeamRecord &br);

// This MACRO must be outside any namespaces.

BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_BeamRecord, gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION)


#endif /* #ifndef BOONETYPES_H */
