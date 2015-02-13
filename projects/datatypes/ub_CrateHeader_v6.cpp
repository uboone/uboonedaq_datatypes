#include "ub_CrateHeader_v6.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

ub_CrateHeader_v6::ub_CrateHeader_v6():
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {0},
                         card_count {0},
                         event_number {0},
                         frame_number {0},
                         gps_time {},
                         daqClock_time {},
                         seb_time_sec {0},
                         seb_time_usec {0}
{}

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_TPC_CardHeader_v6 cardHeader):
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {0},
                         card_count {0},
                         event_number {cardHeader.getEvent()},
                         frame_number {cardHeader.getFrame()},
                         gps_time {},
                         daqClock_time {},
                         seb_time_sec {0},
                         seb_time_usec {0}
{}

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_PMT_CardHeader_v6 cardHeader):
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {0},
                         card_count {0},
                         event_number {cardHeader.getEvent()},
                         frame_number {cardHeader.getFrame()},
                         gps_time {},
                         daqClock_time {},
                         seb_time_sec {0},
                         seb_time_usec {0}
{}

std::string ub_CrateHeader_v6::debugInfo()const
{
    std::ostringstream os;

    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << "\n Event Number " << event_number;
    os << "  Frame Number " <<  frame_number;
    os << "  Crate Number " << (unsigned int)crate_number;
    os << "  Crate Bits " << (unsigned int) crateBits;
    os << "  Crate Complete " << (unsigned int) complete;
    os << "  Crate seb_time_sec " << (unsigned int)seb_time_sec;
    os << "  Crate seb_time_usec " << (unsigned int) seb_time_usec;
    if(crate_number == 10) {
        os << "  \n Crate 10 gps_time (sec, micro, nano) " << gps_time.second << ",  "
        << gps_time.micro << ",  " << gps_time.nano ;
        os << "  \n Crate 10 daqClockTime (frame, sample, div) " << (unsigned int) daqClock_time.frame
        << ", " << (unsigned int) daqClock_time.sample << ", " << (unsigned int)daqClock_time.div;
    }
    os << "\n  Event Size " << size;

    return os.str();
}


ub_CrateHeader_v6 const& ub_CrateHeader_v6::getHeaderFromFragment(raw_data_containter<raw_data_type> const& fragment)
{
    artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* fragment.begin());

    if(artdaq_fragment_header::num_words()<=fragment.size())
        throw datatypes_exception("Invalid fragment: fragment is too short.");

    if(artdaq_header->word_count!=fragment.size())
        throw datatypes_exception("Invalid fragment: fragment length doesn't match header word count.");

    if(!artdaq_header->sequence_id)
        throw datatypes_exception("Invalid fragment: sequence_id is not set");

    if(!artdaq_header->metadata_word_count !=artdaq_fragment_header::padded_size_of<ub_CrateHeader_v6>() )
        throw datatypes_exception("Invalid fragment: wrong metadata_word_count, or wrong header version");

    ub_CrateHeader_v6 const* crate_header =reinterpret_cast<ub_CrateHeader_v6 const *>(&*(fragment.begin()+artdaq_fragment_header::num_words()));

    if(crate_header->size + artdaq_fragment_header::num_words() +artdaq_header->metadata_word_count != fragment.size())
        throw datatypes_exception("Invalid crate header: wrong data size.");

    return *crate_header;
}


bool ub_CrateHeader_v6::compare(ub_CrateHeader_v6 const& crate_header,bool do_rethrow) const throw(datatypes_exception)
{
    try
    {
        if(!data_transmission_header.compare(crate_header.data_transmission_header))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "data_transmission_header", "data_transmission_header","data_transmission_header"));

        if(!complete!=crate_header.complete)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "complete", complete,crate_header.complete));

        if(!crateBits!=crate_header.crateBits)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crateBits", to_hex_string(crateBits),to_hex_string(crate_header.crateBits)));

        if(!size!=crate_header.size)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "size", size,crate_header.size));

        if(!crate_number!=crate_header.crate_number)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crate_number", crate_number,crate_header.crate_number));

        if(!card_count!=crate_header.card_count)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "card_count", card_count,crate_header.card_count));

        if(!crate_type!=crate_header.crate_type)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crate_type", crate_type,crate_header.crate_type));

        if(!event_number!=crate_header.event_number)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "event_number", event_number,crate_header.event_number));

#if 0   //FIXME:GAL implement compare memeber functions and uncomment this block         
        if(!gps_time.compare(crate_header.gps_time))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "gps_time", gps_time,crate_header.gps_time));

        if(!daqClock_time.compare(crate_header.daqClock_time))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "daqClock_time", daqClock_time,crate_header.daqClock_time));
#endif
        if(!seb_time_sec!=crate_header.seb_time_sec)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "seb_time_sec", seb_time_sec,crate_header.seb_time_sec));

        if(!seb_time_usec!=crate_header.seb_time_usec)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "seb_time_usec", seb_time_usec,crate_header.seb_time_usec));

    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception.";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception.");
        else
            return false;
    }
    return true;
}


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}




