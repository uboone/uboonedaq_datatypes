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
  try{
  while(true){
    boost::archive::binary_iarchive ia ( is );      
    ub_EventRecord  eventRecord;
    ia >> eventRecord;
    std::cout << "+++++ Event: " << ++i << "\n";
    std::cout << eventRecord.debugInfo();        
  
  }
  }catch(...){
  std::cout << "Done.";
  }
}
