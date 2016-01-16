#include "ub_PMT_CardData_v6.h"
#include <mutex>
#include <utility>

using namespace gov::fnal::uboone::datatypes;

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

std::once_flag flagpmtcs;

template<>
bool ub_MarkedRawCardData<ub_PMT_ChannelData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::_do_dissect=true;

template<>
bool ub_MarkedRawCardData<ub_PMT_ChannelData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::isValid() noexcept
{
    bool returnIsValid{true};
        
    if(_do_dissect)
    {
	std::call_once(flagpmtcs, [](){ganglia::Metric<ganglia::RATE,uint32_t>::named("PMT-checksum-error-count","Errors/sec")->publish(0);});
        if(!verify_checksum( data(), header().getChecksum() -0x4000)) {
            std::cerr << "Wrong checksum.\n";
	     ganglia::Metric<ganglia::RATE,uint32_t>::named("PMT-checksum-error-count","Errors/sec")->publish(1);            
            _validChecksum=false;
            returnIsValid=true; //continue for now
        }
    }
    return returnIsValid;
}



}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

uint32_t ub_PMT_CardData_v6::getTrigFrame() const noexcept
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

bool ub_PMT_CardData_v6::compare(ub_PMT_CardData_v6 const& card_data,bool do_rethrow ) const throw(datatypes_exception)
{
    try {
        return ub_MarkedRawCardData<ub_PMT_ChannelData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>::compare(card_data,do_rethrow);
    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_PMT_CardData_v6::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_PMT_CardData_v6::compare()");
        else
            return false;
    }
    return true;
}

ub_PMT_CardData_v6::ub_PMT_CardData_v6(ub_RawData const& rawdata):
    ub_MarkedRawCardData<ub_PMT_ChannelData_v6,ub_PMT_CardHeader_v6,ub_PMT_CardTrailer_v6>(rawdata) {}

bool ub_PMT_CardData_v6::operator==(ub_PMT_CardData_v6 const& card_data) const {
    return compare(card_data,false);
}

uint32_t ub_PMT_CardData_v6::getCardTrigFrameAndSampleWord() const noexcept {
    return header().trig_frame_and_sample;
}

uint32_t ub_PMT_CardData_v6::getID() const noexcept {
    return header().getID();
}
uint32_t ub_PMT_CardData_v6::getModule() const noexcept {
    return header().getModule();
}
uint32_t ub_PMT_CardData_v6::getEvent() const noexcept {
    return header().getEvent();
}
uint32_t ub_PMT_CardData_v6::getFrame() const noexcept {
    return header().getFrame();
}
uint32_t ub_PMT_CardData_v6::getChecksum() const noexcept {
    return header().getChecksum();
}
uint32_t ub_PMT_CardData_v6::getWordCount() const noexcept {
    return header().getWordCount();
}
uint32_t ub_PMT_CardData_v6::getTrigFrameMod16() const noexcept {
    return header().getTrigFrameMod16();
}
uint32_t ub_PMT_CardData_v6::getTrigSample() const noexcept {
    return header().getTrigSample();
}
uint16_t ub_PMT_CardData_v6::getDataStartMarker() const noexcept {
    return header().getDataStartMarker();
}
uint16_t ub_PMT_CardData_v6::getDataEndMarker() const noexcept {
    return trailer().getDataEndMarker();
}

uint32_t ub_PMT_CardData_v6::rollOver(uint32_t ref,
				      uint32_t subject) const
{
  // Return "ref" which lower "nbits" are replaced by that of "subject"
  // Takes care of roll over effect.
  // For speed purpose we only accept pre-defined nbits values.
  const unsigned int nbits = 3;
  unsigned int diff=3; // max diff should be (2^(nbits)-2)/2
  unsigned int mask=0x7; // mask to extract lower nbits from subject ... should be 2^(nbits)-1
  
  subject = ( (ref>>nbits) << nbits) + (subject & mask);
  //std::cout<<ref<<" : "<<subject<<" : "<<nbits<< " : "<<diff<<std::endl;
  // If exactly same, return
  if(subject == ref) return subject;
  
  // If subject is bigger than ref by a predefined diff value, inspect difference

  else if ( subject > ref && (subject - ref) > diff)
    
    subject = subject - (mask + 1);
  
  // If subject is smaller than ref by a predefined diff value, inspect difference
  else if ( subject < ref && (ref - subject) > diff)
    
    subject = subject + (mask + 1);
  
  return subject;
}

void ub_PMT_CardData_v6::fillBeamWindowWaveforms(const uint32_t hw_trigger_frame,
						 const uint32_t hw_trigger_sample,
						 std::vector<std::vector<uint16_t> >& result) const
{
  if(result.empty()) return;
  //std::cout<<"Searching for trigger frame " << hw_trigger_frame << " sample " << hw_trigger_sample << std::endl;
  uint64_t target_time =0;
  uint64_t min_dt = 1e12; //FIXME this should be set to max integer value from compiler
  static const uint32_t frame_size = 102400;
  const uint64_t trigger_time = hw_trigger_frame * frame_size + hw_trigger_sample;
  //std::cout << "Trigger in tick @ " << trigger_time << std::endl; 
  // First search the target timing
  for(auto const& ch_data : getChannels()){

    for(auto const& window : ch_data.getWindows()) {

      if(window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM && 
	 window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM_GATE) 
	continue; //ignore non-BEAM signals
      
      uint64_t window_time = rollOver(this->getFrame(), window.header().getFrame()) * frame_size;
      window_time += window.header().getSample();
      //std::cout << "Beamgate @ " << window_time << " size " << window.data().size() <<std::endl;
      uint64_t window_trigger_dt = 
	( window_time < trigger_time ? trigger_time - window_time : window_time - trigger_time );
      
      if( min_dt > window_trigger_dt ) {
	min_dt      = window_trigger_dt;
	target_time = window_time;
      }
    }
  }
  //std::cout << "target_time: " << target_time << std::endl;
  // Reaching here, my_frame && my_sample is non-zero, 
  // then you found a candidate beam gate start (i.e. size == expected size & closest to trigger)
  if(!target_time) {
    //std::cout << "Could not locate beam gate window!!!" << std::endl;
    return;
  }
  
  for(auto const& chan : getChannels()) {

    if(chan.getChannelNumber() >= (int)(result.size())) continue;  //ignore non-PMT channels

    for(auto const& window : chan.getWindows()) {

      uint64_t window_time = rollOver(this->getFrame(), window.header().getFrame()) * frame_size;
      window_time += window.header().getSample();

      // Exactly same timing
      if(window_time == target_time) {

	auto& wf = result[chan.getChannelNumber()];
	if(wf.size() > window.data().size()) {
	  std::cerr << "\033[93m"
		    << "For channel " << chan.getChannelNumber() 
		    << " window data size " << window.data().size()
		    << " smaller than requested waveform size " << wf.size()
		    << "\033[00m"
		    << std::endl;
	  throw datatypes_exception("fuck you");
	}
	for(size_t adc_index=0; adc_index<wf.size(); ++adc_index)
	  wf[adc_index] = (0xfff & (*(window.data().begin()+adc_index)));
	break;
      }
       
      // Concatinated waveform
      if( target_time > window_time && 
	  target_time < (window_time + window.data().size()) ) {

        //std::cout << "window_time: " <<	window_time << ", window_size: " << window.data().size() << std::endl;

	auto& wf = result[chan.getChannelNumber()];
	if(wf.size() + (target_time - window_time) > window.data().size()) {
	  std::cerr << "\033[93m"
		    << "For channel " << chan.getChannelNumber() 
		    << " window data size " << window.data().size()
		    << " smaller than requested waveform size " << wf.size()
		    << "\033[00m"
		    << std::endl;
	  throw datatypes_exception("fuck you");
	}
	for(size_t adc_index=0; adc_index<wf.size(); ++adc_index)
	  wf[adc_index] = (0xfff & (*(window.data().begin()+adc_index+(target_time - window_time))));
	break;
      }
    }
  }

}
