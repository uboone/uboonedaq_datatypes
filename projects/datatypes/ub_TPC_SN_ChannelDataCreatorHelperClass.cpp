#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_TPC_SN_ChannelData_v6.h"
#include <mutex>
#include <iostream>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

std::once_flag flagtpcmcht, flagtpcch;

template<>
void ub_ChannelDataCreatorHelperClass<ub_TPC_SN_ChannelData_v6>::populateChannelDataVector(std::vector<ub_TPC_SN_ChannelData_v6> & channelDataVector)
{
  std::vector<ub_TPC_SN_ChannelData_v6> retValue;
  std::size_t tpc_card_channel_count = 64;    
  retValue.reserve(tpc_card_channel_count);    

  ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};
  uint16_t next_header;
        
  //report missing channel trailer words
  auto missing_trailer_counter= uint32_t{0};
  auto channel_per_event_counter= uint32_t{0};
  

  std::call_once(flagtpcmcht, [](){ganglia::Metric<ganglia::RATE,uint32_t>::named("TPC-missing-channel-trailer-rate","Count/sec")->publish(0);});
  std::call_once(flagtpcch, [](){ganglia::Metric<ganglia::VALUE,uint32_t>::named("TPC-channel-per-event","Channels/event")->publish(64);});

  // std::cout << "Unpack SN card" << std::endl;
  try{
    uint16_t framenumber = (*curr_rawData.begin() & 0xfff) >> 6;
    next_header = 0x1000 + (framenumber << 6); // Channel 0 is the first one we expect.
    
    for(size_t channel=0; channel < tpc_card_channel_count; channel++)
    {
      uint16_t headerword = *curr_rawData.begin();
      if(headerword!=next_header)
      {
        std::stringstream ss;
        ss << "Junk data: Wrong channel header: (channel, expected, received)=(" <<hex(4,channel) << ", ";
        ss << hex(4,next_header) << ", " <<hex(4,*(curr_rawData.begin()));
        ss << "); remaining data size=" << std::dec << curr_rawData.size();
        throw datatypes_exception(ss.str());
      } else {
        std::cout << "--Looked for " << hex(4,headerword) << " and got it." << std::endl;
      }
      next_header++;

      ub_RawData::const_iterator curr_position=curr_rawData.begin();

      curr_position++;  // Move one forward from current header.
      while( (curr_position != curr_rawData.end()) && 
             (((*curr_position)&0xF000)!=0x1000) ){ 
            curr_position++;    
      };
      std::cout << "End of card with word " << std::hex << "0x" << *curr_position <<  " pos: " << curr_position << " end: " << curr_rawData.end() << std::endl;

      ub_RawData data {curr_rawData.begin(),curr_position};                            
      retValue.push_back(data);
      // Fixme: add try..catch around next line so a single bad packet doesn' invalidate the whole card.
      retValue.rbegin()->dissectPackets(); // break up the packets.
      ++channel_per_event_counter;
      curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
                
      if(curr_rawData.size()==0) { 
        std::cout << "Unpack SN Card: Premature end of channel data.  Channel number " << std::dec << channel << std::endl;
        break;
      }

    }//end loop over n_channels
    std::cout << "Got a total of " << std::dec << retValue.size() << " channels." << std::endl;;
    
    ganglia::Metric<ganglia::RATE,uint32_t>::named("TPC-missing-channel-trailer-rate","Count/sec")->publish(missing_trailer_counter);
    ganglia::Metric<ganglia::VALUE,uint32_t>::named("TPC-channel-per-event","Channels/event")->publish(channel_per_event_counter);

    channelDataVector.swap(retValue);
  } catch(datatypes_exception& e) {
    std::cerr << "Caught datatype exception in ub_TPC_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
    // std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
    std::cerr << "Current data block: " << std::endl;
    std::cerr << debugInfo(curr_rawData) << std::endl;

    if(retValue.size()>0){
      std::cerr << "\nPrevious data block: " << std::endl;
      std::cerr << debugInfo(retValue.back().rawdata()) << std::endl;
    }
    std::cerr << "\nRaw Card Data"<< std::endl;
    std::cerr << debugInfo(_rawData) << std::endl;
    throw;
  } catch(std::exception& e){         
    std::cerr << "Caught std exception in ub_TPC_SN_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
    throw datatypes_exception(std::string("Caught datatype exception in ub_TPC_SN_ChannelDataCreatorHelperClass::populateChannelDataVector() Message: ").append(e.what()));
  } catch(...){         
    std::cerr << "Caught unknown exception in ub_TPC_SN_ChannelDataCreatorHelperClass::populateChannelDataVector()" << std::endl;
    throw datatypes_exception("Caught unknown exception in ub_TPC_SN_ChannelDataCreatorHelperClass::populateChannelDataVector()");
  }    
}
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
