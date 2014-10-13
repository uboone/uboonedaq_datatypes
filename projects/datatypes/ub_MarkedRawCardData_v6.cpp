#include "ub_MarkedRawCardData_v6.h"

using namespace gov::fnal::uboone::datatypes;

ub_MarkedRawCardData_v6::ub_MarkedRawCardData_v6(ub_RawData_t const& rd){

  size_t iter_fwd = sizeof(ub_CardHeader_t_v6)/rd.typeSize();
  size_t iter_rwd = sizeof(ub_CardTrailer_t_v6)/rd.typeSize();
  _marked_raw_data_block = 
    ub_MarkedDataBlock<card_header_t_v6,card_trailer_t_v6>( rd,
							    ub_RawData_t(rd.begin()+iter_fwd,rd.end()-iter_rwd),
							    rd.begin(),
							    rd.end()-iter_rwd );

}

uint32_t ub_MarkedRawCardData_v6::getID() const 
{ return (((getCardIDAndModuleWord()>>16) & 0xfff)>>5) & 0x7f;

uint32_t ub_MarkedRawCardData_v6::getModule() const
{ return (getCardIDAndModuleWord()>>16) & 0x1f; }

uint32_t ub_MarkedRawCardData_v6::getEvent() const
{ return ((getCardEventWord()>>16) & 0xfff) + ((getCardEventWord()& 0xfff) <<12); }

uint32_t ub_MarkedRawCardData_v6::getFrame() const
{ return ((getCardFrameWord()>>16) & 0xfff) + ((getCardFrameWord() & 0xfff) <<12); }

uint32_t ub_MarkedRawCardData_v6::getChecksum() const
{ return ((bt_card_header.checksum &0xFFF) << 12) + ((bt_card_header.checksum >> 16)&0xFFF);

uint32_t ub_MarkedRawCardData_v6::getWordCount() const
{ return (((bt_card_header.word_count>>16) & 0xfff)+((bt_card_header.word_count & 0xfff)<<12) ); }

uint32_t ub_MarkedRawCardData_v6::getTrigFrame() const
{
  // Attempt to resolve the rollover situation: the lower 4 bits are given by the trigger frame, which should crudely match the upper bits from the course frame. 
  // Here is how I resolved it --- Nathaniel
  uint32_t frameCourse = getFrame();
  uint32_t option1 = (getFrame()&0xFFFFFFF0) | (getTrigFrameMod16());
  int32_t diff = option1-frameCourse;
  if(diff > 8) return option1 - 0x10; // Solution is too high; rollover down
  if(diff < 8) return option1 + 0x10; // Solution is too low; rollover up.
  return option1; // if within 8 ticks, this solution is fine.
}

uint32_t ub_MarkedRawCardData_v6::getTrigFrameMod16() const
{ return ((getCardTrigFrameAndSampleWord()>>4) & 0xF); }

uint32_t ub_MarkedRawCardData_v6::getTrigSample() const
{ return (getCardTrigFrameAndSampleWord() >> 16) & 0xFF) + ((getCardTrigFrameAndSampleWord() & 0xF)<<8); }
