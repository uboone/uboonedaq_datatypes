#ifndef _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CARDDATACREATORHELPERCLASS_H 1

#include "uboonetypes_Base.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_CardData;
class ub_MarkedRawCardData;

class ub_CardDataCreatorHelperClass{
 public:
  ub_CardDataCreatorHelperClass(ub_VersionWord_t const, ub_RawData_t const);
  std::vector<ub_CardData> getCardDataVector() { return _cardDataVector; }

 private:
  std::vector<ub_CardData> _cardDataVector;
  std::unique_ptr<ub_MarkedRawCardData> _markedRawCardData;

};


}
}
}
}

#endif
