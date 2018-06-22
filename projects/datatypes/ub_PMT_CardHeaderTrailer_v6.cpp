#include "ub_PMT_CardHeaderTrailer_v6.h"

using namespace gov::fnal::uboone::datatypes;

std::string ub_PMT_CardHeader_v6::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << std::hex << std::setfill('0') << std::setw(4);
    os << "Module[" << (int)getModule() << "], ID[" << (int)getID() << "], Marker[" << getMark() << "], RAW[0x" ;    
    os << std::setw(8)<< id_and_module << "]" << std::endl;
    
    os << std::hex << std::setfill('0') << std::setw(4);
    os << "WordCount[" << getWordCount() <<   "], RAW[0x" << word_count <<   "]" << std::endl;
    os << "Event[" << getEvent() << "], RAW[0x" << event_number << "]" << std::endl;
    os << "Frame[" << getFrame() << "], RAW[0x" << frame_number << "]" << std::endl;
    os << "Checksum[" << getChecksum() <<    "], RAW[0x" << checksum <<     "]" << std::endl;
    os << "TrigSample[" << getTrigSample() <<  "], RAW[0x" << trig_frame_and_sample << "]" << std::endl;
    os << "TrigFrameMod16[" << getTrigFrameMod16() <<  "], RAW[0x" << trig_frame_and_sample << "]" << std::endl;
    os << "DataStartMarker[" << getDataStartMarker() << "], RAW[0x" << raw_data_start_marker << "]" << std::endl;

    return os.str();
}

std::string ub_PMT_CardTrailer_v6::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << std::hex << std::setfill('0') << std::setw(4);
    os << "DataEndMarker[" << getDataEndMarker() << "], RAW[0x" << raw_data_end_marker << "]" << std::endl;

    return os.str();
}
