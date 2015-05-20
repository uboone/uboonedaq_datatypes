#ifndef _UBOONE_TYPES_MARKEDTRIGGERCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDTRIGGERCARDDATA_V6_H 1

#include "ub_MarkedRawCardData.h"
#include "ub_Trigger_ChannelData_v6.h"
#include "ub_Trigger_DataTypes_v6.h"
#include "ub_CrateHeader_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_Trigger_CardData_v6 final: public ub_MarkedRawCardData<ub_Trigger_ChannelData_v6,ub_Trigger_Header_v6,empty_trailer> {
public:
    typedef ub_CrateHeader_v6 ub_CrateHeader;

    ub_Trigger_CardData_v6(ub_RawData const& rawdata);
    bool compare(ub_Trigger_CardData_v6 const&,bool do_rethrow=false) const throw(datatypes_exception);
    bool operator==(ub_Trigger_CardData_v6 const& card_data) const;

    uint32_t const& getCardTrigFrameAndSampleWord() const noexcept;
    uint32_t getTrigNumber() const noexcept;
    uint32_t getFrame() const noexcept;
    uint16_t getSample() const noexcept;
    uint16_t getRemainder() const noexcept;

    ub_Trigger_Data_v6 const& getTriggerData() const throw(datatypes_exception);

    ub_Trigger_CardData_v6 ( ub_Trigger_CardData_v6  && ) = default;
    
    static constexpr auto typeName="TRIGGER";

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif //_UBOONE_TYPES_MARKEDTRIGGERCARDDATA_V6_H
