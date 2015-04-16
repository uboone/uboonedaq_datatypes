#include "uboone_data_fragment.h"
#include "uboone_data_internals.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include  <iomanip>

using namespace gov::fnal::uboone::datatypes;

raw_data_containter<raw_data_type> readFile(std::string const& fileName)
{
    std::ostringstream os;

    os << "Reading event data file " << fileName;
    std::ifstream file(fileName, std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if(-1 == fileSize)
    {
        os << ". Exception: file is missing." << std::endl;
        std::cerr <<os.str() <<std::flush;
        throw std::runtime_error(os.str());
    }

    os << ", size " << fileSize << " bytes";
    raw_data_containter<raw_data_type> retBuff(fileSize/sizeof(raw_data_type));
    file.read((char*)&retBuff[0], retBuff.size()*sizeof(raw_data_type));
    os << " into buffer " << std::hex <<  &retBuff << std::endl;

    std::cout <<os.str()<<std::flush;
    return retBuff;
}

