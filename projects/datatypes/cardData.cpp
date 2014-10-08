#include "cardData.h"

using namespace gov::fnal::uboone::datatypes;

void cardData::FillChannelLocations(){
  
  if( channelLocationsFilled() ){
    std::cout << "==========================================================================" << std::endl;
    std::cout << "WARNING: Channel locations previously filled. Will delete and refill." << std::endl;
    std::cout << "==========================================================================" << std::endl;
  }

  channel_locations_vector.clear();

  if(data_vector.size()==0){
    std::cout << "==========================================================================" << std::endl;
    std::cout << "ERROR!" << std::endl;
    std::cout << "\tData vector size is zero!" << std::endl;
    std::cout << "==========================================================================" << std::endl;
    throw std::runtime_error("Data vector size zero.");
  }

  if( data_vector[0]&0xF000 != 0x4000 ){
    std::cout << "==========================================================================" << std::endl;
    std::cout << "ERROR!" << std::endl;
    std::cout << "\tFirst element of data not beginning channel word!  " 
	      << std::hex << "0x" << data_vector[0] << std::dec << std::endl;
    std::cout << "==========================================================================" << std::endl;
    throw std::runtime_error("Bad channel header.");
  }

  bool finish_channel=true;
  uint16_t current_channel_number=0;
  RawDataIterator_t begin_channel_it=data_vector.cbegin();
  for(RawDataIterator_t it=data_vector.cbegin(); it<data_vector.cend(); it++){

    if(*it&0xF000 == 0x4000){

      if(*it&0xFFF != current_channel_number+1){
	std::cout << "==========================================================================" << std::endl;
	std::cout << "ERROR!" << std::endl;
	std::cout << "\tBegin channel number not incremented properly! " 
		  << "Previous channel=" << current_channel_number ", new channel=" << (*it&0xFFF) << std::endl;
	std::cout << "==========================================================================" << std::endl;
	channel_locations_vector.clear();
	throw std::runtime_error("Bad channel header.");
      }

      current_channel_number = *it&0x0FFF;
      begin_channel_it = it;
      finish_channel=false;
    }//end if begin channel word

    else if(*it&0xF000 == 0x5000){

      if(*it&0xFFF != current_channel_number){
	std::cout << "==========================================================================" << std::endl;
	std::cout << "ERROR!" << std::endl;
	std::cout << "\tEnd channel number does not match beginning! " 
		  << "Channel " << current_channel_number "!=" << (*it&0xFFF) << std::endl;
	std::cout << "==========================================================================" << std::endl;
	channel_locations_vector.clear();
	throw std::runtime_error("Bad channel trailer.");
      }
      
      channel_locations_vector.push_back(begin_channel_it);
      finish_channel=true;

    }//end if end channel word

  }//end loop over data vector


  if(!finish_channel){
    std::cout << "==========================================================================" << std::endl;
    std::cout << "ERROR!" << std::endl;
    std::cout << "\tLast channel missing its trailer!"
    std::cout << "==========================================================================" << std::endl;
    channel_locations_vector.clear();
    throw std::runtime_error("Bad channel trailer.");
  }

}//end FillChannelLocations

