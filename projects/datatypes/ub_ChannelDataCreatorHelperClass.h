#ifndef _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H
#define _UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template<typename MRCD>
class ub_ChannelDataCreatorHelperClass {
public:
    ub_ChannelDataCreatorHelperClass(ub_RawData const& rd) :_rawData(rd) {}
    void populateChannelDataVector(std::vector<MRCD> & channelDataVector) ;
private:
    const ub_RawData _rawData;
};

template<typename MRCD>
void ub_ChannelDataCreatorHelperClass<MRCD>::populateChannelDataVector(std::vector<MRCD> & channelDataVector)
{
    static constexpr size_t tpc_card_channel_count = 64;
    ub_RawData curr_rawData {_rawData};
    uint16_t curr_header {0x4000},curr_trailer {0x5000};
    std::vector<MRCD> retValue;
    retValue.reserve(tpc_card_channel_count);
    for(size_t channel=0; channel < tpc_card_channel_count; channel++,curr_trailer++)
    {
        if(*curr_rawData.begin()!=curr_header++)
            throw std::runtime_error("Junk data: Wrong channel header.");

        for(ub_RawData::const_iterator curr_position=curr_rawData.begin(); curr_position!=curr_rawData.end(); curr_position++)
        {
            if(curr_trailer==*curr_position && (curr_header==*(curr_position+1) || channel+1 == tpc_card_channel_count ))
            {
                ub_RawData data {curr_rawData.begin(),curr_position+1};
                retValue.emplace_back( data );
                curr_rawData=ub_RawData {curr_position+1,curr_rawData.end()};
                if(curr_rawData.size()!=0 && channelDataVector.back().minsize() >curr_rawData.size())
                    throw std::runtime_error("Junk data: Corrupt channel data.");
                break;
            }
        }
    }
    channelDataVector.swap(retValue);
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_CHANNELDATACREATORHELPERCLASS_H
