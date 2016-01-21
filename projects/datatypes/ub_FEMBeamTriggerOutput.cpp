#include "ub_FEMBeamTriggerOutput.h"

using namespace gov::fnal::uboone::datatypes;

std::string ub_FEMBeamTriggerOutput::debugInfo() const noexcept
{

    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n Triggered: " << pass;
    os << "\n PHMAX sum: " << amplitude;
    os << "\n Multiplicity: " << multiplicity;
    os << "\n Trigger decision time: " << time;

    return os.str();
}
