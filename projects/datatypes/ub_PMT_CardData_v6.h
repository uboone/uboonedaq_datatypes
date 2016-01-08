#ifndef _UBOONE_TYPES_MARKEDPMTCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDPMTCARDDATA_V6_H 1

#include "ub_MarkedRawCardData.h"
#include "ub_PMT_WindowData_v6.h"
#include "ub_PMT_ChannelData_v6.h"
#include "ub_PMT_CardHeaderTrailer_v6.h"
#include "ub_CrateHeader_v6.h"

#include <limits>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

 class FEMBeamTriggerOutput {
    
    public:
    
      /// Default ctor
    FEMBeamTriggerOutput(const size_t nwindows=0) //change here, originally has nwindows as argument
      : window_start_v ( nwindows,  0 ) //orignally has (nwindow,0) I don't think we need this, it seems it's just a fix number all the time
     , window_end_v    ( nwindows,  0 ) //same as the start?
     , vmaxdiff        ( nwindows,  0 )   
     , vmaxhit         ( nwindows,  0 )     
     , fire_time_v     ( nwindows, -1 )  
     {}
      
      /// Default dtor
      ~FEMBeamTriggerOutput() {}
      
      std::vector <size_t> window_start_v; ///< Window start tick
      std::vector <size_t> window_end_v;   ///< Window end   tick
      std::vector <short>  vmaxdiff;       ///< PHMAX sum vector
      std::vector <short>  vmaxhit;        ///< Multiplicity sum vector
      std::vector <int>    fire_time_v;    ///< Trigger decision times (-1 means not fired)
      
    };


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

    //FEMBeamTriggerOutput getCardTriggerValue( size_t i_begin, size_t i_end, uint32_t max_value=std::numeric_limits<uint32_t>::max() ) const noexcept;
    FEMBeamTriggerOutput getCardTriggerValue( size_t i_begin, size_t i_end) const noexcept;
    //uint32_t trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave, uint32_t ThreshVal) const noexcept;
    
    FEMBeamTriggerOutput trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave) const noexcept;
    
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
