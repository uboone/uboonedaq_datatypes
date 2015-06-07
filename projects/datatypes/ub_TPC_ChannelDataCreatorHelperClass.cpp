#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_TPC_ChannelData_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


template<>
void ub_ChannelDataCreatorHelperClass<ub_TPC_ChannelData_v6>::populateChannelDataVector(std::vector<ub_TPC_ChannelData_v6> & channelDataVector)
{
    std::size_t tpc_card_channel_count = 64;
    ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};
    uint16_t curr_header {0x4000},curr_trailer {0x5000};
    
    std::vector<ub_TPC_ChannelData_v6> retValue;
    retValue.reserve(tpc_card_channel_count);
    
    try{
    for(size_t channel=0; channel < tpc_card_channel_count; channel++,curr_trailer++)
    {
        if(*curr_rawData.begin()!=curr_header++)
        {
            std::stringstream ss;
            ss << "Junk data: Wrong channel header: (channel, expected, received)=(" <<hex(4,channel) << ", ";
            ss << hex(4,curr_header) << ", " <<hex(4,*(curr_rawData.begin()));
            ss << "); remaining data size=" << std::dec << curr_rawData.size();
            ss << "; channel trailer word=" << hex (4,*(curr_rawData.begin()+curr_rawData.size()));
        
            if( *(curr_rawData.begin()+curr_rawData.size()) == 0x503f){
                std::cerr << ss.str() << std::endl;
                break;
             }
             else
                throw datatypes_exception(ss.str(),"datatypes_exception","tpc",-1,-1,channel);
        }

        for(ub_RawData::const_iterator curr_position=curr_rawData.begin(); curr_position!=curr_rawData.end(); curr_position++)
        {
            if(curr_trailer==*curr_position && (curr_header==*(curr_position+1) || channel+1 == tpc_card_channel_count ))
            {
                ub_RawData data {curr_rawData.begin(),curr_position+1};
                
                retValue.push_back(data);
                
                curr_rawData=ub_RawData {curr_position+1,curr_rawData.end()};
                
                if(curr_rawData.size()!=0 && channelDataVector.back().minsize() >curr_rawData.size())
                    throw datatypes_exception("Junk data: Corrupt or truncated channel data");
                    
                break;
            }
        }
    }
    channelDataVector.swap(retValue);
    } catch(datatypes_exception& e) {
      std::cerr << "Caught datatype exception in ub_TPC_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
      std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
      std::cerr << "Raw Card Data"<< std::endl;
      std::cerr <<  debugInfo(_rawData) << std::endl;
      throw e;
    } catch(std::exception& e){         
      std::cerr << "Caught std exception in ub_TPC_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
      throw e;
    }
    
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
