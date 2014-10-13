#ifndef _UBOONE_TYPES_MARKEDRAWCARDDATA_H
#define _UBOONE_TYPES_MARKEDRAWCARDDATA_H 1

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


}
}
}
}

#endif
