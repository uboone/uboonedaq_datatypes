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

bool readFile(std::string const& fileName, raw_data_container<raw_data_type>& data);

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
  
  try{
    raw_data_container<raw_data_type> data;
    
    if(!readFile(fileName,data))
        throw datatypes_exception(std::string("Unable to read file. (fileName=").append(fileName));
        
    ub_RawData raw_data(data.begin(),data.end());
    
    ub_PMT_CrateData_v6 crate(raw_data);
    
    std::cout <<std::endl <<crate.debugInfo();  
  }catch(...){
  std::cout << "Done.";
  }
}
