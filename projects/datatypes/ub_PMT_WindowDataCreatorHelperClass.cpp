#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_PMT_WindowData_v6.h"
#include "ub_PMT_ChannelData_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


  // Nathaniel's code: convert to channels which contain windows.

  template<>
  void ub_ChannelDataCreatorHelperClass<ub_PMT_ChannelData_v6>::populateChannelDataVector(std::vector<ub_PMT_ChannelData_v6> & channelDataVector)
  {
      std::vector< std::vector<ub_PMT_WindowData_v6> > channelGroups(64);
          
      size_t pmt_expected_window_count = 500;
      ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};
    
    
      try{
        ub_RawData::const_iterator curr_position=curr_rawData.begin();
        while(curr_position!=curr_rawData.end())
          {

            if(curr_rawData.size() < sizeof(ub_PMT_WindowHeader_v6)){
              std::stringstream ss;
              ss << "Junk data: Left with a PMT window header that is too small."; 
              throw datatypes_exception(ss.str());
            }
            if( ((*curr_position>>6)&0xf)!=0 || ((*curr_position>>12)&0x3)!=1 ){
              std::stringstream ss;
              ss << "Junk data: Bad PMT Window Header:\n\t" 
                 << quick_cast<ub_PMT_WindowHeader_v6>(curr_position).debugInfo();
              throw datatypes_exception(ss.str());
            }

            curr_position++;
            while(curr_position!=curr_rawData.end()){

              if( ((*curr_position>>6)&0xf)==0 && ((*curr_position>>12)&0x3)==1 ){
                ub_RawData data {curr_rawData.begin(),curr_position};

                ub_PMT_WindowData_v6 window(data);
                int channel = window.getChannelNumber();
                if((channel<0) || (channel>63)) {
                  std::stringstream ss;
                  ss << "Junk data: Bad PMT Window Header channel number: " << channel << "\n\t" 
                           << window.debugInfo();
                  throw datatypes_exception(ss.str());
                }
                channelGroups[channel].push_back(data);
              
                curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
                break;
              }
              else{
                curr_position++;
              }
            }//end search for next window header
          

          }//end for loop over raw data


      channelDataVector.clear();
      for(int channel=0;channel<channelGroups.size();channel++)  {
        channelDataVector.push_back(ub_PMT_ChannelData_v6(channel,channelGroups[channel]));
      }

      }catch(std::exception& e){         
           std::cerr << "Caught exception in ub_PMT_WindowDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
           std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
          // std::cerr << "Raw Card Data"<< std::endl;         
          // std::cerr <<  debugInfo(_rawData) << std::endl;         
          throw;
      }
    
  }


  /*
  // Wes' code: convert to windows.
template<>
void ub_ChannelDataCreatorHelperClass<ub_PMT_WindowData_v6>::populateChannelDataVector(std::vector<ub_PMT_WindowData_v6> & channelDataVector)
{
    size_t pmt_expected_window_count = 500;
    ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};
    
    std::vector<ub_PMT_WindowData_v6> retValue;
    retValue.reserve(pmt_expected_window_count);
    
    try{
      ub_RawData::const_iterator curr_position=curr_rawData.begin();
      while(curr_position!=curr_rawData.end())
        {

          if(curr_rawData.size() < sizeof(ub_PMT_WindowHeader_v6)){
            std::stringstream ss;
            ss << "Junk data: Left with a PMT window header that is too small."; 
            throw datatypes_exception(ss.str());
          }
          if( ((*curr_position>>6)&0xf)!=0 || ((*curr_position>>12)&0x3)!=1 ){
            std::stringstream ss;
            ss << "Junk data: Bad PMT Window Header:\n\t" 
               << quick_cast<ub_PMT_WindowHeader_v6>(curr_position).debugInfo();
            throw datatypes_exception(ss.str());
          }

          curr_position++;
          while(curr_position!=curr_rawData.end()){

            if( ((*curr_position>>6)&0xf)==0 && ((*curr_position>>12)&0x3)==1 ){
              ub_RawData data {curr_rawData.begin(),curr_position};

              retValue.push_back(data);
              
              curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
              break;
            }
            else{
              curr_position++;
            }
          }//end search for next window header
          

        }//end for loop over raw data

    channelDataVector.swap(retValue);
    }catch(std::exception& e){         
         std::cerr << "Caught exception in ub_PMT_WindowDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
         std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
        // std::cerr << "Raw Card Data"<< std::endl;         
        // std::cerr <<  debugInfo(_rawData) << std::endl;         
        throw;
    }
}
    */
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
