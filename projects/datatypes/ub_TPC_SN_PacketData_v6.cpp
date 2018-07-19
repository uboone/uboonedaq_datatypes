#include "ub_TPC_SN_PacketData_v6.h"

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

  template<>
  bool ub_MarkedRawChannelData<ub_TPC_SN_PacketHeader,empty_trailer>::isValid() noexcept
  {
    // FIXME
    return true;
  }

uint16_t ub_TPC_SN_PacketHeader::sLeaderWords = 7;
     
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

ub_TPC_SN_PacketData_v6::ub_TPC_SN_PacketData_v6(ub_RawData const& rawdata)
: ub_MarkedRawChannelData(rawdata){
}




