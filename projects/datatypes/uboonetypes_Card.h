#ifndef _UBOONE_TYPES_CARD_H
#define _UBOONE_TYPES_CARD_H 1

#include "uboonetypes_Base.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_MarkedRawCardData{
 public:

  static ub_MarkedRawCardData* CreateMarkedRawCardData( ub_VersionWord_t const&, ub_RawData_t const& rawdata);

  virtual ub_RawData_t const& data() const;
  virtual ub_RawData_t const& rawdata(); const;

  virtual uint32_t const& getCardIDAndModuleWord();
  virtual uint32_t const& getCardWordCountWord();
  virtual uint32_t const& getCardEventWord();
  virtual uint32_t const& getCardFrameWord();
  virtual uint32_t const& getCardChecksumWord();
  virtual uint32_t const& getCardTrigFrameAndSampleWord();
  virtual uint32_t getID() const;
  virtual uint32_t getModule() const;
  virtual uint32_t getEvent() const;
  virtual uint32_t getFrame() const;
  virtual uint32_t getChecksum() const;
  virtual uint32_t getWordCount() const;
  virtual uint32_t getTrigFrame() const;
  virtual uint8_t  getTrigFrameMod16() const;
  virtual uint32_t getTrigSample() const;

};


/**Card/Module Header format: (each module sends a header followed by adc data)
first word  {16'hffff,               4'h7,id[6:0],module[4:0]}
2nd word    {4'h7,eventnumber[23:12], 4'h7,eventnumber[11:0]}
3d word     {4'h7,wordcount[23:12],  4'h7,wordcount[11:0]}
4th word    {4'h7,checksum[23:12],   4'h7,checksum[11:0]}
**/
typedef struct card_header_v6
{
  uint32_t id_and_module; 
  uint32_t word_count;  //this is number of 16-bit words.
  uint32_t event_number;  
  uint32_t frame_number;  
  uint32_t checksum;
  uint32_t trig_frame_and_sample;  
}card_header_t_v6;

typedef empty_trailer card_trailer_v6;
typedef empty_trailer_t card_trailer_t_v6;

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
  ub_MarkedRawDataBlock<card_header_t_v6,card_trailer_t_v6> _marked_raw_data_block;
};



}
}
}
}

#endif
