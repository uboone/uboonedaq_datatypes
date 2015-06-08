#ifndef _UBOONE_TYPES_MARKEDTRIGGERCRATEDATA_V6_H
#define _UBOONE_TYPES_MARKEDTRIGGERCRATEDATA_V6_H 1

#include "uboone_data_v6_headers.h"

#include "ub_MarkedRawCrateData.h"

#include "ub_Trigger_DataTypes_v6.h"
#include "ub_Trigger_CardData_v6.h"
#include "uboone_data_fragment_metadata.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

class ub_Trigger_CrateData_v6 final: 
  public ub_MarkedRawCrateData<ub_Trigger_CardData_v6,empty_header,ub_Trigger_Trailer_v6> {
public:
    typedef ub_Trigger_CardData_v6::ub_CrateHeader ub_CrateHeader_t;
    
    ub_Trigger_CrateData_v6(ub_RawData const& rawdata, bool initializeHeaderFromRawData=false);
    bool compare(ub_Trigger_CrateData_v6 const&,bool do_rethrow=false) const throw(datatypes_exception);
    
    ub_Trigger_CrateData_v6 ( ub_Trigger_CrateData_v6  && ) = default;
    
    uint32_t getTrailerWord() const noexcept{
      return trailer().getTrailerWord();
    }

    ub_Trigger_CardData_v6 const& getTriggerCardData() const throw(datatypes_exception);
    ub_Trigger_Header_v6 const&   getTriggerHeader()   const throw(datatypes_exception);
    ub_Trigger_Data_v6 const&     getTriggerData()     const throw(datatypes_exception);

    static constexpr auto typeName="TRIGGER";
private:
    //ub_Trigger_CardData_v6 & getTriggerCardData() throw(datatypes_exception);

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_TYPES_MARKEDTRIGGERCRATEDATA_V6_H

