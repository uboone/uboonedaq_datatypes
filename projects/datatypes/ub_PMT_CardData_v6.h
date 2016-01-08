#ifndef _UBOONE_TYPES_MARKEDPMTCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDPMTCARDDATA_V6_H 1

#include "ub_MarkedRawCardData.h"
#include "ub_PMT_WindowData_v6.h"
#include "ub_PMT_ChannelData_v6.h"
#include "ub_PMT_CardHeaderTrailer_v6.h"
#include "ub_CrateHeader_v6.h"
#include "ub_FEMBeamTriggerOutput.h"

#include <limits>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_PMT_CardData_v6 final: public ub_MarkedRawCardData<ub_PMT_ChannelData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6> {
public:
    typedef ub_CrateHeader_v6 ub_CrateHeader;

    ub_PMT_CardData_v6(ub_RawData const& rawdata);

    bool compare(ub_PMT_CardData_v6 const& card_data,bool do_rethrow=false) const throw(datatypes_exception);
    bool operator==(ub_PMT_CardData_v6 const& card_data) const;

    uint32_t getCardTrigFrameAndSampleWord() const noexcept;

    uint32_t getID() const noexcept;
    uint32_t getModule() const noexcept;
    uint32_t getEvent() const noexcept;
    uint32_t getFrame() const noexcept;
    uint32_t getChecksum() const noexcept;
    uint32_t getWordCount() const noexcept;
    uint32_t getTrigFrameMod16() const noexcept;
    uint32_t getTrigSample() const noexcept;
    uint32_t getTrigFrame() const noexcept;
    uint16_t getDataStartMarker() const noexcept;
    uint16_t getDataEndMarker() const noexcept;

    ub_FEMBeamTriggerOutput getCardTriggerValue( uint16_t hw_trigger_sample, uint32_t hw_trigger_frame, size_t i_begin, size_t i_end) const noexcept;
    ub_FEMBeamTriggerOutput trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave) const noexcept;

    ub_PMT_CardData_v6 ( ub_PMT_CardData_v6  && ) = default;
    
    short Discr0delay = 3;
    short fDiscr3delay = 3;
    
    short fDiscr0threshold = 5;
    short fDiscr3threshold = 10;
    
    short fDiscr0precount = 3;
    
    short fDiscr0deadtime = 6;
    short fDiscr3deadtime = 6;
    
    short fDiscr0width = 6;
    short fDiscr3width = 6;
    
    short fMinReadoutTicks = 500;
    short fFrontBuffer = 20;
    short fWindowSize = 103;

    //short fTriggerWinStartTick = 300;
    short fTriggerWinStartTick = 300;      
    short fTriggerThresMult  = 4;
    short fTriggerThresPHMAX = 80;    
    short fTriggerModuleWinStartTick;
    short fTriggerModuleWindowSize;
    static constexpr auto typeName="PMT";
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
#endif //_UBOONE_TYPES_MARKEDPMTCARDDATA_V6_H
