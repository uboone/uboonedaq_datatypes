#include "ub_FEMBeamTriggerOutput.h"

using namespace gov::fnal::uboone::datatypes;

std::string ub_FEMBeamTriggerOutput::debugInfo() const noexcept
{

    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n Window start tick: " << window_start;
    os << "\n Window end tick: " << window_end;
    os << "\n PHMAX sum vector: " << maxdiff;
    os << "\n Multiplicity sum vector: " << maxhit;
    os << "\n Trigger decision time: " << fire_time;

    return os.str();
}
