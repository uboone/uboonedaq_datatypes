#include "ub_TPC_SN_CardData_v6.h"
#include <mutex>
using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

  extern std::once_flag flagtpccs,flagtpccsd;

// template<>
// bool ub_MarkedRawCardData<ub_TPC_SN_ChannelData_v6,ub_TPC_SN_CardHeader_v6,empty_trailer>::_do_dissect=true;

template<>
bool ub_MarkedRawCardData<ub_TPC_SN_ChannelData_v6,ub_TPC_SN_CardHeader_v6,empty_trailer>::isValid() noexcept
{
    bool returnIsValid{true};

    if(_do_dissect)
    {
      std::call_once(flagtpccs, [](){ganglia::Metric<ganglia::RATE,uint32_t>::named("TPC-checksum-error-count","Errors/sec")->publish(0);});
      std::call_once(flagtpccsd, [](){ganglia::Metric<ganglia::VALUE,int>::named("TPC-checksum-diff","Difference")->publish(0);});
      int checksum_diff = checksum_difference( data(), header().getChecksum() );
        if(checksum_diff!=0) {
	  if( (checksum_diff+0x503f) != 0 ){
            // std::cerr << "Wrong checksum.\n";
	    ganglia::Metric<ganglia::RATE,uint32_t>::named("TPC-checksum-error-count","Errors/sec")->publish(1);            
	    ganglia::Metric<ganglia::VALUE,int>::named("TPC-checksum-diff","Difference")->publish(checksum_diff);            
            _validChecksum=false;
            returnIsValid=true; //continue for now
	  }
	  else
	    ganglia::Metric<ganglia::VALUE,int>::named("TPC-checksum-diff","Difference")->publish(0);            

        }
	else
	  ganglia::Metric<ganglia::VALUE,int>::named("TPC-checksum-diff","Difference")->publish(0);            
	}
    return returnIsValid;
}



}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

uint32_t ub_TPC_SN_CardData_v6::getTrigFrame() const noexcept
{
    // Attempt to resolve the rollover situation: the lower 4 bits are given by the trigger frame, which should crudely match the upper bits from the course frame.
    // Here is how I resolved it --- Nathaniel
    uint32_t frameCourse = getFrame();
    uint32_t option1 = (getFrame()&0xFFFFFFF0) | (getTrigFrameMod16());
    int32_t diff = option1-frameCourse;
    if(diff > 8) return option1 - 0x10; // Solution is too high; rollover down
    if(diff < 8) return option1 + 0x10; // Solution is too low; rollover up.
    return option1; // if within 8 ticks, this solution is fine.
}

bool ub_TPC_SN_CardData_v6::compare(ub_TPC_SN_CardData_v6 const& card_data,bool do_rethrow ) const throw(datatypes_exception)
{
    try {
        return ub_MarkedRawCardData<ub_TPC_SN_ChannelData_v6,ub_TPC_SN_CardHeader_v6,empty_trailer>::compare(card_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_TPC_SN_CardData_v6::compare()\n";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_TPC_SN_CardData_v6::compare()");
        else
            return false;
    }
    return true;
}


ub_TPC_SN_CardData_v6::ub_TPC_SN_CardData_v6(ub_RawData const& rawdata):
    ub_MarkedRawCardData<ub_TPC_SN_ChannelData_v6,ub_TPC_SN_CardHeader_v6,empty_trailer>(rawdata)
{
}

bool ub_TPC_SN_CardData_v6::operator==(ub_TPC_SN_CardData_v6 const& card_data) const {
    return compare(card_data,false);
}

uint32_t const& ub_TPC_SN_CardData_v6::getCardTrigFrameAndSampleWord() const noexcept {
    return header().trig_frame_and_sample;
}

uint32_t ub_TPC_SN_CardData_v6::getID() const noexcept {
    return header().getID();
}
uint32_t ub_TPC_SN_CardData_v6::getModule() const noexcept {
    return header().getModule();
}
uint32_t ub_TPC_SN_CardData_v6::getEvent() const noexcept {
    return header().getEvent();
}
uint32_t ub_TPC_SN_CardData_v6::getFrame() const noexcept {
    return header().getFrame();
}
uint32_t ub_TPC_SN_CardData_v6::getChecksum() const noexcept {
    return header().getChecksum();
}
uint32_t ub_TPC_SN_CardData_v6::getWordCount() const noexcept {
    return header().getWordCount();
}
uint32_t ub_TPC_SN_CardData_v6::getTrigFrameMod16() const noexcept {
    return header().getTrigFrameMod16();
}
uint32_t ub_TPC_SN_CardData_v6::getTrigSample() const noexcept {
    return header().getTrigSample();
}
