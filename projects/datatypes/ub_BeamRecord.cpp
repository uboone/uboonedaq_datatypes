#include "ub_BeamRecord.h"

using namespace gov::fnal::uboone::datatypes;

ub_BeamHeader_t const& ub_BeamRecord::beamHeader() const noexcept{
    return _beam_header;
}

void ub_BeamRecord::setBeamHeader (ub_BeamHeader_t const& beam_header) noexcept{
    _beam_header = beam_header;
}

ub_BeamDataVector_t const& ub_BeamRecord::beamDataVector() const noexcept{
    return _beam_data_vector;
}

void ub_BeamRecord::clearBeamDataVector() noexcept{
    _beam_data_vector.clear();
}

void ub_BeamRecord::insertBeamData(ub_BeamData_t const& beam_data) noexcept{
    _beam_data_vector.push_back(beam_data);
}
