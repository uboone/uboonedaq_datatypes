#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_Trigger_ChannelData_v6.h"
#include "ub_Trigger_DataTypes_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<>
void ub_ChannelDataCreatorHelperClass<ub_Trigger_ChannelData_v6>::populateChannelDataVector(std::vector<ub_Trigger_ChannelData_v6> & channelDataVector)
{
  try{
    if(_rawData.size() != size_of<ub_Trigger_Data_v6>()){
      std::stringstream ss;
      ss << "Junk data: Trigger Data is not right size. "; 
      ss << _rawData.size() << " vs " << size_of<ub_Trigger_Data_v6>();
      throw datatypes_exception(ss.str());
    }
    
    std::vector<ub_Trigger_ChannelData_v6> retValue;
    retValue.push_back(_rawData);
    
    channelDataVector.swap(retValue);
  }catch(std::exception& e){         
    std::cerr << "Caught exception in ub_Trigger_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
    std::cerr <<  debugInfoShort(_rawData) << std::endl;
    std::cerr << "Raw Card Data"<< std::endl;         
    std::cerr <<  debugInfo(_rawData) << std::endl;         
    throw;
  }
 }
    
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
