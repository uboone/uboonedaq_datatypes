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
                         gps_time {0,0,0},
                         trigger_board_time {0,0,0},
                         local_host_time {0,0}                         
{}

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_TPC_CardHeader_v6 const& cardHeader):
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {cardHeader.getID()},
                         card_count {0},
                         event_number {cardHeader.getEvent()},
                         frame_number {cardHeader.getFrame()},
                         gps_time {0,0,0},
                         trigger_board_time {0,0,0},
                         local_host_time {0,0}
{}
  
ub_CrateHeader_v6::ub_CrateHeader_v6(ub_TPC_SN_CardHeader_v6 const& cardHeader):
      data_transmission_header {},
                           complete {0},crateBits {0},
                           size {0},
                           crate_number {cardHeader.getID()},
                           card_count {0},
                           event_number {cardHeader.getEvent()},
                           frame_number {cardHeader.getFrame()},
                           gps_time {0,0,0},
                           trigger_board_time {0,0,0},
                           local_host_time {0,0}
  {}  

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_PMT_CardHeader_v6 const& cardHeader):
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {cardHeader.getID()},
                         card_count {0},
                         event_number {cardHeader.getEvent()},
                         frame_number {cardHeader.getFrame()},
                         gps_time {0,0,0},
                         trigger_board_time {0,0,0},
                         local_host_time {0,0}
{}

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_Trigger_Header_v6 const& header):
    data_transmission_header {},
                         complete {0},crateBits {0},
                         size {0},
                         crate_number {100},
                         card_count {0},
                         event_number {header.getTriggerNumber()},
                         frame_number {header.getFrame()},
                         gps_time {0,0,0},
                         trigger_board_time {header.getFrame(),
			                     header.get2MHzSampleNumber(),
			                     header.get16MHzRemainderNumber()},
                         local_host_time {0,0}
{}

ub_CrateHeader_v6::ub_CrateHeader_v6(ub_LaserData const& laserdata)
  :data_transmission_header {},
  complete {1},crateBits {0},
  size {size_of<ub_LaserData>()},
  //crate_number {200+(uint8_t)laserdata.getID()},
  card_count {1},
  crate_type { SystemDesignator::LASER_SYSTEM },
  //event_number {(unsigned int)laserdata.getCountTrigger()},
  frame_number {0},
  gps_time {0,0,0},
  trigger_board_time {0,0,0},
  local_host_time {0,0}
{
  if(laserdata.getID()<0 || laserdata.getID()>(1<<8))
    throw datatypes_exception(std::string("Caught exception in ub_CrateHeader_v6 constructor. Bad laser ID"));
  uint8_t id = laserdata.getID() % (1<<8);
  crate_number = 200 + id;

  if(laserdata.getCountTrigger()<0)
    throw datatypes_exception(std::string("Caught exception in ub_CrateHeader_v6 constructor. Bad trigger counter"));
  event_number = (unsigned int)laserdata.getCountTrigger();

}
  
void ub_CrateHeader_v6::copyIn(ub_CrateHeader_v6 const& source)  noexcept
{
    *this=source;
}

uint64_t ub_CrateHeader_v6::sequenceID() const noexcept
{
  //return frame_number;
  return event_number;
}

void ub_CrateHeader_v6::copyOut(ub_CrateHeader_v6&  target)  noexcept
{
    target=*this;
}

void ub_CrateHeader_v6::updateCrateBits() noexcept
{
    number=crate_number;
    type=crate_type;
    reserved=0;
}

std::string ub_CrateHeader_v6::debugInfo()const noexcept
{
    std::ostringstream os;

    os << "\n Event Number: " << event_number;
    os << "\n   Event Size: " << size;
    os << "\n   Frame Number: " <<  frame_number;
    os << "\n   Crate Number: " << int(crate_number);
    os << "\n   Crate Bits: " << "0x" << std::hex << crateBits;
    os << "\n   Crate Complete: " <<  (complete ? "True":"False");
    os << "\n   " << data_transmission_header.debugInfo();
    os << "\n   " << local_host_time.debugInfo();

    if(crate_number==10){
        os << "\n   " << gps_time.debugInfo();
        os << "\n   " << trigger_board_time.debugInfo();
    }


    return os.str();
}

void ub_CrateHeader_v6::updateDTHeader(ub_RawData const& data)
{
    assert(artdaq_fragment_header::num_words()<data.size());
    
    if(artdaq_fragment_header::num_words()>data.size())
        throw datatypes_exception("Invalid fragment: fragment is too short.");
        

    artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&*data.begin());

    if(artdaq_header->word_count!=data.size())
        throw datatypes_exception("Invalid fragment: fragment length doesn't match header word count.");
        
    if(artdaq_header->metadata_word_count !=artdaq_fragment_header::padded_wordcount_of<ub_CrateHeader_v6>() )
        throw datatypes_exception("Invalid fragment: wrong metadata_word_count, or wrong header version");
    
    data_transmission_header.total_fragment_wordcount=artdaq_header->word_count;
    data_transmission_header.is_fragment_complete=complete;
    
    data_transmission_header.fragment_format_version= gov::fnal::uboone::datatypes::constants::DATATYPES_VERSION;
    
    data_transmission_header.raw_fragment_beginning_word_offset=
      artdaq_fragment_header::num_words()+
      artdaq_fragment_header::padded_wordcount_of<ub_CrateHeader_v6>();
      
    data_transmission_header.raw_fragment_wordcount=
      data_transmission_header.total_fragment_wordcount-
      data_transmission_header.raw_fragment_beginning_word_offset; 

    data_transmission_header.calculateMD5hash((unsigned char const* )&*data.begin(),data.size()*sizeof(raw_data_type));
  
}

ub_CrateHeader_v6 const& ub_CrateHeader_v6::getHeaderFromFragment(ub_RawData const& data)
{
  try
  {
    assert(artdaq_fragment_header::num_words()<data.size());
    
    if(artdaq_fragment_header::num_words()>data.size())
        throw datatypes_exception("Invalid fragment: fragment is too short.");

    artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* data.begin());

    if(artdaq_header->word_count!=data.size())
        throw datatypes_exception("Invalid fragment: fragment length doesn't match header word count.");
    /*
    if(!artdaq_header->sequence_id)
        throw datatypes_exception("Invalid fragment: sequence_id is not set");
    */
    if(artdaq_header->metadata_word_count !=artdaq_fragment_header::padded_wordcount_of<ub_CrateHeader_v6>() )
        throw datatypes_exception("Invalid fragment: wrong metadata_word_count, or wrong header version");

    ub_CrateHeader_v6 const* crate_header =reinterpret_cast<ub_CrateHeader_v6 const *>(&*(data.begin()+artdaq_fragment_header::num_words()));

    if(crate_header->data_transmission_header.raw_fragment_wordcount + artdaq_fragment_header::num_words() +artdaq_header->metadata_word_count != data.size())
        throw datatypes_exception("Invalid crate header: wrong data size.");

    return *crate_header;
    
    } catch(datatypes_exception &ex) {
            throw datatypes_exception(std::string("Caught exception in ub_CrateHeader_v6::getHeaderFromFragment(). Message:").append(ex.what()));
    } catch(...) {
            throw datatypes_exception("Unknown exception in ub_CrateHeader_v6::getHeaderFromFragment()");
    }
}


bool ub_CrateHeader_v6::compare(ub_CrateHeader_v6 const& crate_header,bool do_rethrow) const
{
    try
    {
        if(!data_transmission_header.compare(crate_header.data_transmission_header))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "data_transmission_header", "data_transmission_header","data_transmission_header"));

        if(complete!=crate_header.complete)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "complete", complete,crate_header.complete));

        if(crateBits!=crate_header.crateBits)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crateBits", to_hex_string(crateBits),to_hex_string(crate_header.crateBits)));

        if(size!=crate_header.size)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "size", size,crate_header.size));

        if(crate_number!=crate_header.crate_number)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crate_number", crate_number,crate_header.crate_number));

        if(card_count!=crate_header.card_count)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "card_count", card_count,crate_header.card_count));

        if(crate_type!=crate_header.crate_type)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "crate_type", crate_type,crate_header.crate_type));

        if(event_number!=crate_header.event_number)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "event_number", event_number,crate_header.event_number));

#if 0   //FIXME:GAL implement compare memeber functions and uncomment this block         
        if(!gps_time.compare(crate_header.gps_time))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "gps_time", gps_time,crate_header.gps_time));

        if(!trigger_board_time.compare(crate_header.trigger_board_time))
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "trigger_board_time", trigger_board_time,crate_header.trigger_board_time));

        if(!local_host_time!=crate_header.local_host_time)
            throw datatypes_exception(make_compare_message("ub_CrateHeader_v6", "local_host_time"));
#endif


    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw datatypes_exception(std::string("Caught exception in ub_CrateHeader_v6::compare(). Message:").append(ex.what()));
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_CrateHeader_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_CrateHeader_v6::compare()");
        else
            return false;
    }
    return true;
}

bool ub_CrateHeader_v6::isValidChecksum() const noexcept
{
  return data_transmission_header.isValidChecksum();
}


}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}




