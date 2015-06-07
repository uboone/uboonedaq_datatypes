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

using namespace gov::fnal::uboone::datatypes;



int main(int argc, char **argv)
{
  std::string fileName;
  int eventNumber(-12);
  int nevents(0);
  
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

  try{
    is.seekg(-6,is.end); // Go to position 6 bytes before end of file. 
    uint8_t buff[10];
    is.read((char*) buff, 6);
    nevents = *(uint32_t*)(buff);
    if (nevents>0 && nevents <1E9) closedCleanly = true; 
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

    while(true){
      boost::archive::binary_iarchive ia ( is );      
      ub_EventRecord  eventRecord;
      ia >> eventRecord;
      ++i;
      if (i != eventNumber) continue;
      std::cout << "+++++ Event: " << i << "\n";


      global_header_t const& globalHeader = eventRecord.getGlobalHeader();
      std::cout << globalHeader.debugInfo() << std::endl;
      break;
      
    }
  }catch(...){
    std::cout << "Done. closedCleanly?: " << closedCleanly << std::endl;

  }
}
