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
	
class ub_TPC_CardData_v6 : public ub_MarkedRawCardData<ub_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer>{
public: 
  typedef ub_CrateHeader_v6 ub_CrateHeader;

  ub_TPC_CardData_v6(ub_RawData const rawdata): 
  ub_MarkedRawCardData<ub_ChannelData_v6,ub_TPC_CardHeader_v6,empty_trailer>(rawdata){}

  uint32_t const& getCardTrigFrameAndSampleWord() const { return header().trig_frame_and_sample; }

  uint32_t getID() const {return header().getID();}
  uint32_t getModule() const {return header().getModule();}
  uint32_t getEvent() const {return header().getEvent();}
  uint32_t getFrame() const {return header().getFrame();}
  uint32_t getChecksum() const {return header().getChecksum();}
  uint32_t getWordCount() const {return header().getWordCount();}
  uint32_t getTrigFrameMod16() const {return header().getTrigFrameMod16();}
  uint32_t getTrigSample() const {return header().getTrigSample();}
  uint32_t getTrigFrame() const ;  
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
} 
#endif //_UBOONE_TYPES_MARKEDTPCCARDDATA_V6_H
