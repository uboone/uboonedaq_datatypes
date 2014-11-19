#include "ub_MarkedRawChannelData.h"

using namespace gov::fnal::uboone::datatypes;	

bool ub_MarkedRawChannelData::isValid()
{
  if( header().header_mark!=0x40 ) 
		throw std::runtime_error("Invalid Raw Channel Data: header word found.");
  if( trailer().trailer_mark!=0x50 ) 
		throw std::runtime_error("Invalid Raw Channel Data: trailer word found.");
  return true;
}
  
std::string ub_MarkedRawChannelData::debugInfo()const
{
	std::ostringstream os;
	os << "Object " << demangle(typeid(this)) << "."<< std::endl;
	os << header().debugInfo();
	os << trailer().debugInfo();
	os <<  ub_MarkedRawDataBlock::debugInfo();
	return os.str();
}