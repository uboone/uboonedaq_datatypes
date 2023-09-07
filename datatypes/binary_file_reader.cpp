#include "uboone_data_fragment.h"
#include "uboone_data_internals.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include  <iomanip>
#include <cstring>


using namespace gov::fnal::uboone::datatypes;

bool readFile(std::string const& fileName,raw_data_container<raw_data_type> & retBuff)
{
    std::ostringstream os;
    try {
    os << "Reading event data file " << fileName;
    std::ifstream file(fileName, std::ios::binary);
    
    if (!file.good())
        throw datatypes_exception(std::string("Unable to open file:").append(fileName));
        
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if(-1 == fileSize) {
        os << ". Exception: file is missing." << std::endl;
        std::cerr <<os.str() <<std::flush;
        return false;
    }

    os << ", size " << fileSize << " bytes";
    retBuff.resize(fileSize/sizeof(raw_data_type));
    file.read((char*)&retBuff[0], retBuff.size()*sizeof(raw_data_type));
    os << " into buffer " << std::hex <<  &retBuff << std::endl;
   } catch(std::exception const& e) {
      std::cerr <<os.str()<<" Message:"<< e.what() <<std::flush;
      throw datatypes_exception(std::string("Caught  std::exception in gov::fnal::uboone::datatypes::readFile() Message: ").append(e.what()));
   }
   
   // std::cout <<os.str()<<std::flush;
    return true;
}

std::streamsize readfakedata(std::string const& fileName,  char* buffer, std::streamsize size )
{
  raw_data_container<raw_data_type> dma_data(0);
  
   if(!readFile(fileName,dma_data))
        return 0;
  
  std::streamsize availableSize(dma_data.size()*sizeof(raw_data_type));
  
  if(size < availableSize )
    return 0;
    
  std::memcpy ( buffer, (char*) &*dma_data.begin(),availableSize);    
  return availableSize ;  
}
