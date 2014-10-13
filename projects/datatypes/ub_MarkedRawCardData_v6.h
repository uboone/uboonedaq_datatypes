#ifndef _UBOONE_TYPES_MARKEDRAWCARDDATA_V6_H
#define _UBOONE_TYPES_MARKEDRAWCARDDATA_V6_H 1

#include "uboonetypes_Base.h"
#include "ub_MarkedRawCardData.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

/**Card/Module Header format: (each module sends a header followed by adc data)
first word  {16'hffff,               4'h7,id[6:0],module[4:0]}
2nd word    {4'h7,eventnumber[23:12], 4'h7,eventnumber[11:0]}
3d word     {4'h7,wordcount[23:12],  4'h7,wordcount[11:0]}
4th word    {4'h7,checksum[23:12],   4'h7,checksum[11:0]}
**/
typedef struct ub_CardHeader_v6
{
  uint32_t id_and_module; 
  uint32_t word_count;  //this is number of 16-bit words.
  uint32_t event_number;  
  uint32_t frame_number;  
  uint32_t checksum;
  uint32_t trig_frame_and_sample;  
}ub_CardHeader_t_v6;

typedef empty_trailer   ub_CardTrailer_v6;
typedef empty_trailer_t ub_CardTrailer_t_v6;

class ub_MarkedRawCardData_v6 : public ub_MarkedRawCardData{

 public:
  ub_MarkedRawCardData_v6(ub_RawData_t const& rawdata);

  ub_RawData_t const& data() const { return _marked_raw_data_block.data(); }
  ub_RawData_t const& rawdata(); const { return _marked_raw_data_block.rawdata(); }

  uint32_t const& getCardIDAndModuleWord() const { return _marked_raw_data_block.header().id_and_module; }
  uint32_t const& getCardWordCountWord() const { return _marked_raw_data_block.header().word_count; }
  uint32_t const& getCardEventWord() const { return _marked_raw_data_block.header().event_number; }
  uint32_t const& getCardFrameWord() const { return _marked_raw_data_block.header().frame_number; }
  uint32_t const& getCardChecksumWord() const { return _marked_raw_data_block.header().checksum; }
  uint32_t const& getCardTrigFrameAndSampleWord() const { return _marked_raw_data_block.header().trig_frame_and_sample; }

  uint32_t getID() const;
  uint32_t getModule() const;
  uint32_t getEvent() const;
  uint32_t getFrame() const;
  uint32_t getChecksum() const;
  uint32_t getWordCount() const;
  uint32_t getTrigFrame() const;
  uint8_t  getTrigFrameMod16() const;
  uint32_t getTrigSample() const;

 private:
  ub_MarkedRawDataBlock<ub_CardHeader_t_v6,ub_CardTrailer_t_v6> _marked_raw_data_block;
};



}
}
}
}

#endif
