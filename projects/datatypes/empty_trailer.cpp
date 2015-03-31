#include "empty_trailer.h"

using namespace gov::fnal::uboone::datatypes;

std::string empty_trailer::debugInfo()const
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this))<< "."<< std::endl;

    return os.str();
}

