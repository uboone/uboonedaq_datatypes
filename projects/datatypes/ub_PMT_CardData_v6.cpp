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

uint32_t ub_PMT_CardData_v6::getCardTriggerValue( uint16_t hw_trigger_sample,
						  uint32_t hw_trigger_frame,
						  size_t i_begin, size_t i_end, 
						  uint32_t max_value) const noexcept {
  //std::vector< std::pair< std::vector<uint16_t>::const_iterator, std::vector<uint16_t>::const_iterator > > my_waveforms;
  std::vector< std::vector<uint16_t> > my_waveforms;
  uint64_t target_time =0;
  uint64_t min_dt = 1e12; //FIXME this should be set to max integer value from compiler
  static const uint32_t frame_size = 102400;
  static const uint32_t beam_window_size = 1501;  
  const uint64_t trigger_time = hw_trigger_frame * frame_size + hw_trigger_sample;
 
  if(i_begin > beam_window_size || i_end > beam_window_size || i_end < i_begin) {
    std::cout << "please go to hell" << std::endl;
    throw std::exception();
  }
 
  // First search the target timing
  for(auto const& ch_data : getChannels()){

    for(auto const& window : chan.getWindows()) {

      if(window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM && 
	 window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM_GATE) 
	continue; //ignore non-BEAM signals
      
      int64_t window_time = window.header().getFrame() * frame_size + window.header().getSample();

      uint64_t window_trigger_dt = 
	( window_time < trigger_time ? trigger_time - window_time : window_time - trigger_time );
      
      if( min_dt > window_trigger_dt ) {
	min_dt      = window_trigger_dt;
	target_time = window.header().getFrame() * frame_size + window.header().getSample();
      }
    }
  }
  // Reaching here, my_frame && my_sample is non-zero, 
  // then you found a candidate beam gate start (i.e. size == expected size & closest to trigger)
  if(!target_time) {
    std::cout << "Could not locate beam gate window!!!" << std::endl;
    throw std::exception();
  }
  
  for(auto const& chan : getChannels()) {

    if(chan.getChannelNumber()>31) continue;  //ignore non-PMT channels

    for(auto const& window : chan.getWindows()) {

      uint64_t window_time = window.header().getFrame() * frame_size + window.header().getSample();

      // Exactly same timing
      if(window_time == target_time) {
	if(window.data().size() < beam_window_size) {
	  std::cout << "Unexpected readout window (same timing as beamgate but too short)" << std::endl;
	  throw std::exception();
	}
	my_waveforms.emplace_back(window.data().begin()+i_begin,
				  window.data().begin()+i_end);	
	break;
      }
       
      // Concatinated waveform
      if( target_time > window_time && 
	  target_time < (window_time + window.data().size()) ) {
	
	my_waveforms.emplace_back(window.data().begin() + i_begin + (target_time - window_time),
				  window.data().begin() + i_end   + (target_time - window_time) );
	break;
      }
    }
  }

  if(my_waveforms.size() != 32) {
    std::cout << "Lacking some channel..." << std::endl;
    throw std::exception();
  }
  /*
  for(auto const& chan : getChannels()){
    if(chan.getChannelNumber()>31) continue; //ignore non-PMT channels
    for(auto const& window : chan.getWindows()){
      if(window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM && 
	 window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM_GATE) continue; //ignore non-BEAM signals
      if(my_frame==0 && my_sample==0){
	my_frame = window.header().getFrame();
	my_sample = window.header().getSample();
      }
      else if(window.header().getFrame()!=my_frame || window.header().getSample()!=my_sample) break;

      //my_waveforms.emplace_back( std::make_pair(window.data().begin()+i_begin,window.data().begin()+i_end) );
      my_waveforms.emplace_back(window.data().begin()+i_begin,window.data().begin()+i_end);
      
    }
  }
  */

  return trig_thresh_val(my_waveforms,max_value);

  //return max_value;

}

uint32_t ub_PMT_CardData_v6::trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave, uint32_t ThreshVal) const noexcept{

  size_t diff_val = 3;
  std::vector<uint32_t> TotesADC(Wave.at(0).size());
  uint32_t Max = 0;
    
  for (size_t i=0 ; i<Wave.size(); ++i) {
    //cerr<< "i: "<< i << endl;
    for (size_t j=diff_val ; j<Wave[i].size(); ++j) {
            
      //uint16_t ADC = Wave[i][j];
      //TotesADC[j] += ADC;
      if((Wave[i][j-diff_val]&0xfff) > (Wave[i][j]&0xfff))
	continue;

      TotesADC[j] += ( (Wave[i][j]&0xfff) - (Wave[i][j-diff_val]&0xfff));            
      if (TotesADC[j]>ThreshVal) {
	return TotesADC[j];
      }

      if (TotesADC[j]>Max) {
	Max = TotesADC[j];
	//std::cout<< "\t\t\t\tMax: "<< Max << " at " << j << " in waveform " << i << std::endl;
      }
    }
  }
    
  return Max;

}
