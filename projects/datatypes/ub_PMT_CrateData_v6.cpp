#include "ub_PMT_CrateData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <>
bool  ub_MarkedRawCrateData<ub_PMT_CardData_v6>::isValid() noexcept
{
    return true;
}

bool ub_PMT_CrateData_v6::compare(ub_PMT_CrateData_v6 const & crate_data, bool do_rethrow) const throw(datatypes_exception)
{
    try {
        return ub_MarkedRawCrateData<ub_PMT_CardData_v6>::compare(crate_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_PMT_CrateData_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_PMT_CrateData_v6::compare()");
        else
            return false;
    }
    return true;
}

ub_PMT_CrateData_v6::ub_PMT_CrateData_v6(ub_RawData const rawdata, bool createHeaderFromData):
        ub_MarkedRawCrateData<ub_PMT_CardData_v6>(rawdata, createHeaderFromData) {
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

