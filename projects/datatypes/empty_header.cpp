#include "empty_header.h"

using namespace gov::fnal::uboone::datatypes;

std::string empty_header::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this))<< "."<< std::endl;

    return os.str();
}

