#include <iosfwd>
#include <fstream> 
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "datatypes/raw_data_access.h"
#include <boost/archive/binary_iarchive.hpp>
#include "datatypes/ub_EventRecord.h"

using namespace gov::fnal::uboone::datatypes;



int main(int argc, char **argv)
{
  std::string fileName;
  int eventNumber{1};
  
  switch(argc-1)
  {
  case 2:
    eventNumber=atoi(argv[2]);    
  case 1:
    fileName=(argv[1]);    
  default:
    ;
  }
  assert(!fileName.empty());
  
  std::ifstream is ( fileName , std::ios::binary | std::ios::in);
  int i{0};
  try{
  while(true){
    boost::archive::binary_iarchive ia ( is );      
    ub_EventRecord  eventRecord;
    ia >> eventRecord;
   
    if(++i!=eventNumber)
      continue;      
      
    std::cout << "+++++ Exporting Event: " << eventRecord.getGlobalHeader().getEventNumber() << "\n";
    std::cout << eventRecord.getGlobalHeader().debugInfo() << "\n";
  
    auto stream_sync=[](std::ofstream& os){  
      #ifndef __clang__
      std::filebuf* filebuf =os.rdbuf();  
      class my_filebuf : public std::filebuf{
      public:
	int handle() { return _M_file.fd(); }};
      #endif
      os.flush();
      #ifndef __clang__
      fsync(static_cast<my_filebuf*>(filebuf)->handle());
      #endif
    };

   
    for(auto const& seb: eventRecord.getTPCSEBMap()){    
        std::ofstream os ( 
            std::string(fileName).
            append(".event-").append(std::to_string(eventRecord.getGlobalHeader().getEventNumber())).
            append(".crate-").append(std::to_string(seb.first)).append(".rawfragment") , 
            std::ios::binary | std::ios::out);
            
        os.write((char*) &*seb.second.rawdata().begin(),seb.second.rawdata().size_bytes());        
        stream_sync(os);        
        os.close();
    }
    
    for(auto const& seb: eventRecord.getPMTSEBMap()){    
        std::ofstream os ( 
            std::string(fileName).
            append(".event-").append(std::to_string(eventRecord.getGlobalHeader().getEventNumber())).
            append(".crate-").append(std::to_string(seb.first)).append(".rawfragment") , 
            std::ios::binary | std::ios::out);
            
        os.write((char*) &*seb.second.rawdata().begin(),seb.second.rawdata().size_bytes());
        stream_sync(os);        
        os.close();
    }
    
    break;
  }
  }catch(...){
  std::cout << "Done.";
  }
  
  std::cout << "Done.";
}
