#ifndef _UBOONE_TYPES_MARKEDTPCCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDTPCCARDDATA_V6_H 1

#include "ub_MarkedRawCardData.h"
#include "ub_ChannelData_v6.h"
#include "ub_TPC_CardHeader_v6.h"
#include "ub_CrateHeader_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_TPC_CardData_v6 : public ub_MarkedRawCardData<ub_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer> {
public:
    typedef ub_CrateHeader_v6 ub_CrateHeader;

    ub_TPC_CardData_v6(ub_RawData const rawdata):
        ub_MarkedRawCardData<ub_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer>(rawdata) {}
    bool compare(ub_TPC_CardData_v6 const&,bool do_rethrow=false) const throw(datatypes_exception);
    bool operator==(ub_TPC_CardData_v6 const& card_data) const {
        return compare(card_data,false);
    }
    uint32_t const& getCardTrigFrameAndSampleWord() const throw() {
        return header().trig_frame_and_sample;
    }

    uint32_t getID() const throw() {
        return header().getID();
    }
    uint32_t getModule() const throw() {
        return header().getModule();
    }
    uint32_t getEvent() const throw() {
        return header().getEvent();
    }
    uint32_t getFrame() const throw() {
        return header().getFrame();
    }
    uint32_t getChecksum() const throw() {
        return header().getChecksum();
    }
    uint32_t getWordCount() const throw() {
        return header().getWordCount();
    }
    uint32_t getTrigFrameMod16() const throw() {
        return header().getTrigFrameMod16();
    }
    uint32_t getTrigSample() const throw() {
        return header().getTrigSample();
    }
    uint32_t getTrigFrame() const throw();
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif //_UBOONE_TYPES_MARKEDTPCCARDDATA_V6_H
