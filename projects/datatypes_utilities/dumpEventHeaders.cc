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

#include "datatypes/raw_data_access.h"
#include <boost/archive/binary_iarchive.hpp>
#include "datatypes/ub_EventRecord.h"
#include "datatypes/uboone_data_utils.h"
using namespace gov::fnal::uboone::datatypes;



int main(int argc, char **argv)
{
  std::string fileName;
  int eventNumber(-12);
  int nevents(0);

  // Some file-record-hopping code stolen/repurposed from online_monitoring/DaqFile.
  
  switch(argc-1)
  {
  case 2:
    fileName=(argv[1]);    
    eventNumber=std::atoi(argv[2]);    
    break;
  default:
    std::cout << "usage: dumpEventHeaders filename eventNumber" << std::endl;
    return 1;
  }
  assert(!fileName.empty());
  assert(eventNumber>=0);
  
  std::ifstream is ( fileName , std::ios::binary | std::ios::in);
  int i{0};
  bool closedCleanly (false);
  std::vector<uint32_t>  index_buffer;  // sizes of each event
  int status = 1;

  peek_at_next_event<ub_TPC_CardData_v6>(false);
  peek_at_next_event<ub_PMT_CardData_v6>(false);
  handle_missing_words<ub_TPC_CardData_v6>(true);
  handle_missing_words<ub_PMT_CardData_v6>(true);

  try{
    is.seekg(-6,is.end); // Go to position 6 bytes before end of file. 
    uint8_t buff[10];
    is.read((char*) buff, 6);
    uint32_t* buff_ptr = (uint32_t*)(buff);
    nevents = *buff_ptr;
    uint16_t endOfFileMarker = *(uint16_t*)(buff+4);
    if(endOfFileMarker == 0xe0f0 && (nevents>0 && nevents <1E9) )
      closedCleanly = true; 
   
    uint64_t index_pos = 6 + sizeof(uint32_t)*nevents;
    is.clear();
    index_buffer.resize(nevents);
    is.seekg(-index_pos,is.end);
    is.read((char*)(&index_buffer[0]),sizeof(uint32_t)*nevents);   
    is.seekg(0,is.beg);
   
  }
  catch(...)
  {
    std::cout << "Bad trailer. Proceeding to try to find your chosen event, nevertheless." << std::endl;
  }

  try{
    is.clear();
    is.seekg(0,is.beg);

    std::cout << " There are " << std::dec << nevents << " events in this file." << std::endl;
    if (eventNumber > nevents) eventNumber = nevents;

    if (!closedCleanly) // walk evt-by-evt
      {
	while(true){
	  boost::archive::binary_iarchive ia ( is );      
	  ub_EventRecord  eventRecord;
	  ia >> eventRecord;
	  ++i;
	  if (i != eventNumber) continue;
	  std::cout << "eventRecord.size is " << sizeof(eventRecord) << std::endl;
	  global_header_t const& globalHeader = eventRecord.getGlobalHeader();
	  std::cout << globalHeader.debugInfo() << std::endl;
	  break;
	}
	std::cout << "+++++ Event: " << i << "\n";
	status = 0;
      }

    else // ffwd to desired event
      {
	boost::archive::binary_iarchive ia ( is );      
	ub_EventRecord  eventRecord;
	size_t offset (0);
	for(int j=0;j<(eventNumber-1);j++) 
	  { offset += index_buffer[j];
	  }
	is.seekg(offset,is.cur);

	std::cout << " Offset is " << offset << std::endl;
	ia >> eventRecord;
	std::cout << "sizeof(eventRecord) is " << sizeof (eventRecord) << std::endl;
	global_header_t const& globalHeader = eventRecord.getGlobalHeader();
	std::cout << globalHeader.debugInfo() << std::endl;
	status = 0;
	
      }


  }catch(...){
    std::cout << "Done. closedCleanly?: " << closedCleanly << std::endl;
    status = 2;
  }

  return status;
}
