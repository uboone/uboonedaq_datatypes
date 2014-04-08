#ifndef _UBOONETYPES_CARDUTILITIES_H
#define _UBOONETYPES_CARDUTILITIES_H

/***
    This is for common utilities to aid in looking at serealized data
    (or perhaps raw binary data too...?). Things that are meant to modify
    data products should not go here. It's also likely things that are meant to
    unpack or decode data products need not go here.
 ***/

#include "datatypes/cardHeader.h"
#include "datatypes/cardData.h"
#include "datatypes/channelData.h"
#include "datatypes/cardHeaderPMT.h"
#include "datatypes/cardDataPMT.h"
#include "datatypes/channelDataPMT.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
namespace utilities {

  using namespace gov::fnal::uboone::datatypes;

  //gives difference between header and sum of card data
  long compareCheckSum(cardHeader    const& cH, cardData    const& cD);
  long compareCheckSum(cardHeaderPMT const& cH, cardDataPMT const& cD);
  
  uint32_t getCheckSumFromDataBlock(const char* data_ptr,size_t data_size);
  uint32_t getCheckSumFromChannel(channelData const& chD);
  uint32_t getCheckSumFromChannel(channelDataPMT const& chD); // FIXME doesn't work

}
}
}
}
}

#endif
