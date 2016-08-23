#include "uboone_data_fragment.h"
#include "uboone_data_utils.h"
#include <sstream>

using namespace gov::fnal::uboone::datatypes;

datatypes_exception::datatypes_exception(std::string const& message, std::string const& name
  // ,const std::string& crate_type, int crate, int card, int channel
  ) :_name(name), _message(message)
      // ,_crate_type(crate_type), _crate(crate), _card(card), _channel(channel)
{
    std::stringstream out;
    out << " " << _name << " Message: " << _message <<".";
    _message = out.str();
}

const char * datatypes_exception::what () const noexcept 
{
    return _message.c_str();
}

datatypes_exception::~datatypes_exception() noexcept
{

}

data_size_exception::data_size_exception(size_t const& size, std::string const& message, std::string const& name)
  : _name(name), _message(message), _size(size)
{
    std::stringstream out;
    out << " " << _name << " Size Missing: " << _size << " Message: " << _message <<".";
    _message = out.str();
}
//data_size_exception::data_size_exception(std::string const& message, std::string const& name){
//{
//  data_size_exception(0,message,name);
//}

const char * data_size_exception::what () const noexcept 
{
    return _message.c_str();
}

data_size_exception::~data_size_exception() noexcept
{

}

// datatypes_exception& datatypes_exception::operator=(const datatypes_exception& e)
// {
//   _name        = e._name      ;
//   _message     = e._message   ;
//   _crate_type  = e._crate_type;
//   _crate       = e._crate     ;
//   _card        = e._card      ;
//   _channel     = e._channel   ;
//   return *this;
// }


bool ub_fragment_header::compare(ub_fragment_header const& header, bool do_rethrow) const throw(datatypes_exception)
{
    try {
        if(total_fragment_wordcount!=header.total_fragment_wordcount)
            throw datatypes_exception(make_compare_message("data_transmission_header", "total_fragment_wordcount", total_fragment_wordcount,header.total_fragment_wordcount));

        if(fragment_format_version!=header.fragment_format_version)
            throw datatypes_exception(make_compare_message("data_transmission_header", "fragment_format_version", fragment_format_version,header.fragment_format_version));

        if(is_fragment_complete!=header.is_fragment_complete)
            throw datatypes_exception(make_compare_message("data_transmission_header", "is_fragment_complete", is_fragment_complete,header.is_fragment_complete));

        if(raw_fragment_wordcount!=header.raw_fragment_wordcount)
            throw datatypes_exception(make_compare_message("data_transmission_header", "raw_fragment_wordcount", raw_fragment_wordcount,header.raw_fragment_wordcount));

        if(raw_fragment_beginning_word_offset!=header.raw_fragment_beginning_word_offset)
            throw datatypes_exception(make_compare_message("data_transmission_header", "raw_fragment_beginning_word_offset", raw_fragment_beginning_word_offset,header.raw_fragment_beginning_word_offset));

        if(md5hash!=header.md5hash)
            throw datatypes_exception(make_compare_message("data_transmission_header", "md5hash", md5hash,header.md5hash));
            
        if(extra_flags!=header.extra_flags)
            throw datatypes_exception(make_compare_message("data_transmission_header", "extra_flags", extra_flags,header.extra_flags));
            
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in  ub_fragment_header::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in  ub_fragment_header::compare()");
        else
            return false;
    }
    return true;
}


void ub_fragment_header::calculateMD5hash(unsigned char const* addr, std::size_t bytes) noexcept {
    assert(addr!=NULL);
    
#ifdef __APPLE__    
    CC_MD5(addr, bytes , md5hash);
#else
    MD5(addr, bytes , md5hash);
#endif

}

bool ub_fragment_header::verifyMD5hash(unsigned char const* addr, std::size_t bytes) const noexcept {
    assert(addr!=NULL);

    unsigned char md5hash_[MD5_DIGEST_LENGTH];
#ifdef __APPLE__ 
    CC_MD5(addr, bytes , md5hash_);
#else
    MD5(addr, bytes , md5hash_);
#endif

    return std::equal(std::begin(md5hash), std::end(md5hash), std::begin(md5hash_));
}


void ub_fragment_header::flagChecksumAsInvalid() noexcept{
    SET_BIT(extra_flags,0);
}

bool ub_fragment_header::isValidChecksum() const noexcept{
    return ! CHECK_BIT(extra_flags,0);
}


std::string ub_fragment_header::debugInfo() const noexcept {

      std::ostringstream os;      
      os << "Object " << demangle(typeid(this)) << ".";
      os << "\n Total word count: " <<   total_fragment_wordcount;
      os << "\n Fragment format version: " <<   int (fragment_format_version);
      os << "\n Raw data word count: " <<   raw_fragment_wordcount;
      os << "\n Raw data begining offset: " <<   raw_fragment_beginning_word_offset;
      os << "\n Is complete: " <<   (is_fragment_complete?"True":"False");      
      os << "\n   Extra flags:"   <<"0x" << std::hex << extra_flags;
      os << "\n       Checksum:"  << (isValidChecksum()?"Valid":"Invalid");
      //os << "\n       XMIT Trailer:"  << (isMissingXMITTrailer()?"Missing":"OK");
      os << "\n MD5:"  ; for(int i=0; i < MD5_DIGEST_LENGTH; i++) os << std::hex <<std::setw(1)<< (int)md5hash[i];

      return os.str();

}
