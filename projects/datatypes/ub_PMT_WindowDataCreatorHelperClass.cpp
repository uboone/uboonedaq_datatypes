#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_ChannelDataCreatorHelperClass.h"
#include "ub_PMT_WindowData_v6.h"
#include "ub_PMT_ChannelData_v6.h"
#include <iostream>
#include <vector>
#include <deque>
namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


  // Nathaniel's code: convert to channels which contain windows.

  template<>
  void ub_ChannelDataCreatorHelperClass<ub_PMT_ChannelData_v6>::populateChannelDataVector(std::vector<ub_PMT_ChannelData_v6> & channelDataVector)
  {
      std::vector< std::vector<ub_PMT_WindowData_v6> > channelGroups(48); // 48 of them on an FEM.
          
      //size_t pmt_expected_window_count = 500;
      ub_RawData curr_rawData {_rawData.begin(),_rawData.end()};

      // 
      // Local variable to handle a minor fifo overflow problem (i.e. not drastic/catastrophic and data going into next frame)
      //
      //std::deque<unsigned int> bt_words; // KAZU DEBUG TOOL: commented out for production
      std::vector<unsigned int> ch_word_ctr_v; // Channel word counter: keeps track of what channel has appeared in data stream or not
      bool next_channel_data=false; // Boolean to skip data from the previous frame before 4-readout frames
      bool first_data=true; // Boolean to mark whether very first data word is readout or not=> used for ignoring data words from previous frame
      try{
        ub_RawData::const_iterator curr_position=curr_rawData.begin();
        while(curr_position!=curr_rawData.end())
          {
	    /* // KAZU DEBUG TOOL
	    bt_words.push_back((*curr_position));
	    if(bt_words.size()>10) { bt_words.pop_front(); }
	    */
            if(curr_rawData.size() < ub_PMT_WindowHeader_v6::size_words()){
              std::stringstream ss;
              ss << "Junk data: Left with a PMT window header that is too small."; 
              throw datatypes_exception(ss.str());
            }
            if( ((*curr_position>>6)&0x7)!=0 || ((*curr_position>>12)&0xf)!=9 ){
	      if(first_data) next_channel_data = true;
	      else{
		std::stringstream ss;
		ss << "Junk data: Bad PMT Window Header:\n\t"
		   << quick_cast<ub_PMT_WindowHeader_v6>(curr_position).debugInfo();
		/* // KAZU DEBUG TOOL
		ss << "Junk data: Bad PMT Window Header:\n\t" << 
		ss <<"Last 10 bt_words dump:"<<std::endl;
		for(auto const& w : bt_words)
		  ss << std::hex << w << std::dec << " ";
		ss<<std::endl<<"dump done..."<<std::endl;
		*/
		throw datatypes_exception(ss.str());
	      }
	    }
	    if(first_data) first_data = false;

            curr_position++;
            while(curr_position!=curr_rawData.end()){

              if( ((*curr_position>>6)&0x7)==0 && ((*curr_position>>12)&0xf)==9 ){
                ub_RawData data {curr_rawData.begin(),curr_position};
                
                if(data.size() <= ub_PMT_WindowHeader_v6::size_words()){
                  std::stringstream ss;
		  ss << "Concatinated dicsriminator firing, lacking enough header words (size=" << data.size() << ")" << std::endl;
		  if(data.size()) {
		    unsigned int ch = ((*data.begin()) & 0xff);
		    ss << "Channel: " << ch << std::endl;
		    if(ch_word_ctr_v.size() > ch) {
		      ss << "# adcs so far: "<<ch_word_ctr_v[ch]<<std::endl
			 << "Position: " << std::distance(curr_rawData.begin(),curr_position)<<std::endl;
		      for(auto const& v : data)
			ss << "\t" << std::hex<< v << std::dec << std::endl;
		    }
		  }
		  std::cout<<ss.str()<<std::endl;
		  curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
		  break;

                  //ss << "\t"        << debugInfo(curr_rawData);
                  //throw datatypes_exception(ss.str());                  
                }
                
		if(!next_channel_data){
		  ub_PMT_WindowData_v6 window(data);
		  uint16_t channel = window.getChannelNumber();
		  if(channel >= channelGroups.size()) {
		    std::stringstream ss;
		    ss << "Junk data: Bad PMT Window Header channel number: " << channel << "\n\t" 
		       << window.debugInfo();
		    throw datatypes_exception(ss.str());
		  }
		  if(ch_word_ctr_v.size()<= channel) ch_word_ctr_v.resize(channel+1,0);
		  ch_word_ctr_v[channel]+=data.size();
		  channelGroups[channel].push_back(data);
		}else
		  next_channel_data = false;

                curr_rawData=ub_RawData {curr_position,curr_rawData.end()};
                break;
              }
              else{
                curr_position++;
              }
            }//end search for next window header
          

          }//end for loop over raw data


      channelDataVector.clear();
      for(size_t channel=0;channel<channelGroups.size();channel++)  {
        channelDataVector.push_back(ub_PMT_ChannelData_v6(channel,channelGroups[channel]));
      }

      }catch(std::exception& e){         
           std::cerr << "Caught exception in ub_PMT_WindowDataCreatorHelperClass::populateChannelDataVector() Message: " <<e.what() << std::endl;
           std::cerr <<  debugInfoShort(curr_rawData) << std::endl;
           std::cerr << "Raw Card Data"<< std::endl;         
	   printDebugInfo(_rawData,std::cerr);
          throw;
      }catch(...){         
      std::cerr << "Caught unknown exception in ub_PMT_WindowDataCreatorHelperClass::populateChannelDataVector()" << std::endl;
      throw datatypes_exception("Caught unknown exception in ub_PMT_WindowDataCreatorHelperClass::populateChannelDataVector()");
    }        
  }


    
}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}
