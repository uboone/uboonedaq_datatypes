#include <iosfwd>
#include <fstream> 
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <chrono>
#include <random>

#include "datatypes/raw_data_access.h"
#include <boost/archive/binary_iarchive.hpp>
#include "datatypes/ub_EventRecord.h"
#include "datatypes/uboone_data_utils.h"

using namespace gov::fnal::uboone::datatypes;

int main(int argc, char **argv)
{
  peek_at_next_event<ub_TPC_CardData_v6>(false);
  peek_at_next_event<ub_PMT_CardData_v6>(false);
  handle_missing_words<ub_TPC_CardData_v6>(true);
  handle_missing_words<ub_PMT_CardData_v6>(true);  

  std::string fileName;
  
  switch(argc-1)
  {
  case 1:
    fileName=(argv[1]);    
  default:
    ;
  }
  assert(!fileName.empty());
  
  std::ifstream is ( fileName , std::ios::binary | std::ios::in);
  int i{0};
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  try{
  while(true){


    /*
      Boost Readin:
      This is how you read in an event from a file. Note, you MUST make a 
      new binary_iarchive per for every event. The read command advances
      the file stream.

      The basic data unit is the EventRecord. Everything about the event should
      be contained within it.
    */
    boost::archive::binary_iarchive ia ( is );      
    ub_EventRecord  eventRecord;
    ia >> eventRecord;
    std::cout << "+++++ Event: " << ++i << "\n";

    std::chrono::steady_clock::time_point evtrigger_start{std::chrono::steady_clock::now()};
    double random = distribution(generator);
    //uint16_t trig_value = std::numeric_limits<uint16_t>::max();
    FEMBeamTriggerOutput trig_value;
    if(eventRecord.getPMTSEBMap().size()>0){
      auto const& pmt_crate = eventRecord.getPMTSEBMap().begin()->second;
      if(pmt_crate.getCards().size()>0){
	auto const& pmt_card = pmt_crate.getCards().at(0);
	trig_value = pmt_card.getCardTriggerValue(128,384);
	//std::cout << "\t\t\tWe made it in here and see " << trig_value << std::endl;
      }
    }
    std::chrono::steady_clock::time_point evtrigger_end{std::chrono::steady_clock::now()};
    double trigger_time_milliseconds{(double)std::chrono::duration_cast<std::chrono::microseconds>(evtrigger_end-evtrigger_start).count()};

    //std::cout << "\t\tTrigger..." << random << " " << trigger_time_milliseconds << " " << trig_value << std::endl;

    /*
      Debug info:
      You can use this as a utility to print what's in
      the event. I've commented it out for this sample file.
    */
    //std::cout << eventRecord.debugInfo();        

    /*
      Global header:
      Events should contain a global header. To access that, you do the following.
      Note: the global_header_t typedef is defined in raw_data_access.h
      See ub_GlobalHeader.h for the members.
    */
    global_header_t const& globalHeader = eventRecord.getGlobalHeader();
    //std::cout << globalHeader.debugInfo();

    //continue;

    /*
      Accessing SEB fragments:
      Events contain maps of different fragment types (TPC or PMT for the moment).
      You can get the total number of fragments...
    */
    size_t n_total_fragments = eventRecord.getFragmentCount();

    /*
      TPC fragment map:
      The fragments themselves are contained in separate map classes.
      The tpc seb map is an <int,tpc_crate_data_tuple_t> pair. That tuple contains
      ---<0> a raw_data_fragment_t, which is a vector of uint16_t. This is the data!
      ---<1> a ptr to a ub_RawData object, which is the pointer to be used for accessing
             that data.
      ---<2> a ptr to a tpc_crate_data_t, which is a typedef for the TPC Crate data object, 
             which can be used for accessing that data in an interpreted way (something that
	     understands the headers, data format, etc.)
      In most cases, you want access through the last one, as that contains the info needed
      to get to the information directly.
    */

    //This is a range based for loop, where we loop over the elements of the map.
    for(auto const& seb : eventRecord.getTPCSEBMap()){

      int tpc_seb_num = seb.first;
      tpc_crate_data_t const& tpc_crate = seb.second;

      //Note, you can print out the info for that tpc crate!
      //std::cout << tpc_crate.debugInfo();
      
      //The tpc crate data object contains access to a header, trailer, and the internal data.
      //You have to follow the typedefs, but the header and trailer for the TPC crate data are
      //ub_XMITEventHeader and ub_XMITEventTrailer objects. You find the header files for those,
      //and you can find that they are uint32_t types. I'd recommend using auto here to access:
      auto const& tpc_crate_header = tpc_crate.header();
      auto const& tpc_crate_trailer = tpc_crate.trailer();

      //Special to the crate, there is a special header that the DAQ attaches. You can access this
      //like so. The type here is a unique ptr to a ub_CrateHeader_v6 struct. That has useful info
      //like the local host time, which may or may not be set properly right now...
      auto const& tpc_crate_DAQ_header = tpc_crate.crateHeader();
      ub_LocalHostTime this_time = tpc_crate_DAQ_header->local_host_time;
      
      //The Crate Data is split up into Cards. You use the "getCards()" command to get access to
      //each of those. Note that calling this function will dissect the data if it has not already
      //been dissected (debugInfo() calls getCards()). You can do a look over the cards like so:
      for(auto const& card : tpc_crate.getCards()){

	//The format here is similar to the crate! There's a header (which is a ub_TPC_CardHeader_v*
	//object), and technically a trailer (though here it's empty!).
	auto const& tpc_card_header = card.header();
	auto const& tpc_card_trailer = card.trailer();

	//Of course, you can probe for information in the card header. You'll have to find the appropriate
	//header file to know what type you have, but again, these will follow typical practice. And, you
	//can always use debugInfo to not only print the info, but it tells you the type.
	auto const this_event_number = card.getEvent();
	auto const this_frame_number = card.getFrame();


	//And, you guessed it, the tpc card data is split up into one more level: by channel.
	for(auto const& channel : card.getChannels()){

	  //There's a header and trailer here. Remember these are just uint16_t, that contain the
	  //channel number.
	  auto const& tpc_channel_header = channel.header();
	  auto const& tpc_channel_trailer = channel.trailer();

	  //The channel object (ub_MarkedRawChannelData) has a method for returning the channel.
	  //You can look at the other objects too (like ub_MarkedRawCardData) and see methods of
	  //use there as well.
	  auto const tpc_channel_number = channel.getChannelNumber();

	  //And you probably want the data! Well, you can access it like so:
	  auto const& tpc_channel_data = channel.data();

	  //That data is a vector of uint16_t (again, our data is stored that way)
	  for(auto const& tick : channel.data()){
	    //std::cout << tick;
	  }
	  
	}
	
      }
      
    }
    

    
  }
  }
  catch(...){
    std::cout << "Done.";
  }
}
