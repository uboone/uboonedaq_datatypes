#ifndef _UBOONE_TYPES_MARKEDTPCSNCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDTPCSNCARDDATA_V6_H 1

#include "ub_MarkedRawCardData.h"
#include "ub_TPC_SN_ChannelData_v6.h"
#include "ub_TPC_CardHeader_v6.h"
#include "ub_CrateHeader_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_SN_CardData_v6 final: public ub_MarkedRawCardData<ub_TPC_SN_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer> {
public:
    typedef ub_CrateHeader_v6 ub_CrateHeader;

    ub_TPC_SN_CardData_v6(ub_RawData const& rawdata);
    bool compare(ub_TPC_SN_CardData_v6 const&,bool do_rethrow=false) const throw(datatypes_exception);
    bool operator==(ub_TPC_SN_CardData_v6 const& card_data) const;

    uint32_t const& getCardTrigFrameAndSampleWord() const noexcept;
    uint32_t getID() const noexcept;
    uint32_t getModule() const noexcept;
    uint32_t getEvent() const noexcept;
    uint32_t getFrame() const noexcept;
    uint32_t getChecksum() const noexcept;
    uint32_t getWordCount() const noexcept;
    uint32_t getTrigFrameMod16() const noexcept;
    uint32_t getTrigSample() const noexcept;
    uint32_t getTrigFrame() const noexcept;

    ub_TPC_SN_CardData_v6 ( ub_TPC_SN_CardData_v6  && ) = default;
    
    static constexpr auto typeName="TPCSN";

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif //_UBOONE_TYPES_MARKEDTPCCARDDATA_V6_H
