#include "ub_FEMBeamTriggerOutput.h"

using namespace gov::fnal::uboone::datatypes;

std::string ub_FEMBeamTriggerOutput::debugInfo() const noexcept
{

    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n Algorithm: " << algo_instance_name;
    os << "\n Triggered: " << pass;
    os << "\n Pass algo: " << pass_algo;
    os << "\n Pass prescale: " << pass_prescale;
    os << "\n PHMAX sum: " << amplitude;
    os << "\n Multiplicity: " << multiplicity;
    os << "\n Trigger decision time: " << time;
    os << "\n Prescale weight: " << prescale_weight;

    return os.str();
}
