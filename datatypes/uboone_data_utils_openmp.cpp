#include <vector>
#include "uboone_data_utils.h"

using namespace ub_data_types;

#if defined(__clang__)


#include <numeric>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
uint32_t compute_checksum(ub_RawData const& data) noexcept
{
      uint32_t initial_value{0};
      uint32_t return_value{0};
      return_value = std::accumulate(data.begin(), data.end(),initial_value);
      return return_value;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov



#else 

/// For GCC compiler on linux only: 

#include <parallel/numeric>
namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
uint32_t compute_checksum(ub_RawData const& data) noexcept
{
      uint32_t initial_value{0};
      uint32_t return_value{0};         
      #pragma omp parallel //num_threads(16)
      {
	return_value=__gnu_parallel::accumulate(data.begin(), data.end(), initial_value,[](uint32_t a, uint32_t b){return a+b;});
      }
      
      return return_value;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

#endif

