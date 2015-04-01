#include "ub_TPC_CrateData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <>
bool  ub_MarkedRawCrateData<ub_TPC_CardData_v6>::isValid()
{
    //FIXME: GAL add additional validation for the header and trailer
    return true;
}

bool ub_TPC_CrateData_v6::compare(ub_TPC_CrateData_v6 const & crate_data, bool do_rethrow) const throw(datatypes_exception)
{
    try
    {
        return ub_MarkedRawCrateData<ub_TPC_CardData_v6>::compare(crate_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception.";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception.");
        else
            return false;
    }
    return true;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov
