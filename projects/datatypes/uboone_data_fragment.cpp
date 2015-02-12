#include "uboone_data_fragment.h"
#include <sstream> 

using namespace gov::fnal::uboone::datatypes;	

datatypes_exception::datatypes_exception(std::string const& message, std::string const& name)
    :_name(name), _message(message)
{
    std::stringstream out;
    out << " " << _name << " Message: " << _message;
    _message = out.str();
}

const char * datatypes_exception::what () const throw ()
{
    return _message.c_str();
}

datatypes_exception::~datatypes_exception() throw()
{

}

