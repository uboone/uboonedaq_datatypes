#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_PMT_ChannelData_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


template<>
void ub_ChannelDataCreatorHelperClass<ub_PMT_ChannelData_v6>::populateChannelDataVector(std::vector<ub_PMT_ChannelData_v6> & channelDataVector)
{
    std::size_t pmt_card_channel_count = 1;
    ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};
    uint16_t curr_header {0x4000}, curr_trailer {0xc000};
    
    std::vector<ub_PMT_ChannelData_v6> retValue;
    //retValue.reserve(pmt_card_channel_count);//FIXME:GAL
    UNUSED(pmt_card_channel_count);
    try{
    {
        if(*curr_rawData.begin()!=curr_header)
        {
            std::stringstream ss;
            ss << "Junk data: Wrong channel header: (expected, received)=(";
            ss << hex(4,curr_header) << ", " <<hex(4,*(curr_rawData.begin()));
            ss << "); remaining data size=" << std::dec << curr_rawData.size();
            ss << "; channel trailer word=" << hex (4,*(curr_rawData.begin()+curr_rawData.size()));
            
            if( *(curr_rawData.begin()+curr_rawData.size()) == curr_trailer){
                std::cerr << ss.str() << std::endl;
                return;
             }
             else
                throw datatypes_exception(ss.str());
        }

        for(ub_RawData::const_iterator curr_position=curr_rawData.begin(); curr_position!=curr_rawData.end(); curr_position++)
        {
            if(curr_trailer==*curr_position && (EVENTTRAILER == *((decltype(EVENTTRAILER)*)&*(curr_position+1))))
            {
                ub_RawData data {curr_rawData.begin(),curr_position};
                
                retValue.push_back(data);
                
                curr_rawData=ub_RawData (curr_position+1,curr_rawData.end());
                
                if(curr_rawData.size() >= 2 && EVENTTRAILER != *((decltype(EVENTHEADER)*)&*(curr_rawData.begin())))
                    throw datatypes_exception("Junk data: Expecting EVENTHEADER.");                    
                break;
            }
        }
    }
    channelDataVector.swap(retValue);
    }catch(std::exception& e){
         std::cerr << "Caught exception in ub_PMT_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
         std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
         //std::cerr << "Raw Card Data"<< std::endl;         
         //std::cerr <<  debugInfo(_rawData) << std::endl;
        throw;
    }
    
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
