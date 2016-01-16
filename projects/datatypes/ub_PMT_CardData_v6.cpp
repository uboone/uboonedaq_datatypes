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
/*
ub_FEMBeamTriggerOutput ub_PMT_CardData_v6::getCardTriggerValue( uint16_t hw_trigger_sample,
								 uint32_t hw_trigger_frame,
								 size_t i_begin, size_t i_end) const noexcept {

  //std::vector< std::pair< std::vector<uint16_t>::const_iterator, std::vector<uint16_t>::const_iterator > > my_waveforms;
  std::vector< std::vector<uint16_t> > my_waveforms;
  uint64_t target_time =0;
  uint64_t min_dt = 1e12; //FIXME this should be set to max integer value from compiler
  static const uint32_t frame_size = 102400;
  static const uint32_t beam_window_size = 1501;  
  const uint64_t trigger_time = hw_trigger_frame * frame_size + hw_trigger_sample;
 
  if(i_begin > beam_window_size || i_end > beam_window_size || i_end < i_begin) {
    throw datatypes_exception("The configuration of the region of interest of the PMT waveform doesn't make sense!");
  }
 
  // First search the target timing
  for(auto const& ch_data : getChannels()){

    for(auto const& window : ch_data.getWindows()) {

      if(window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM && 
	 window.header().getDiscriminantor()!=ub_PMT_DiscriminatorTypes_v6::BEAM_GATE) 
	continue; //ignore non-BEAM signals
      
      uint64_t window_time = window.header().getFrame() * frame_size + window.header().getSample();

      uint64_t window_trigger_dt = 
	( window_time < trigger_time ? trigger_time - window_time : window_time - trigger_time );
      
      if( min_dt > window_trigger_dt ) {
	min_dt      = window_trigger_dt;
	target_time = window.header().getFrame() * frame_size + window.header().getSample();
      }
    }
  }
  std::cout << "target_time: " << target_time << std::endl;
  // Reaching here, my_frame && my_sample is non-zero, 
  // then you found a candidate beam gate start (i.e. size == expected size & closest to trigger)
  if(!target_time) {
    std::cout << "Could not locate beam gate window!!!" << std::endl;
    return ub_FEMBeamTriggerOutput();
  }
  
  for(auto const& chan : getChannels()) {

    if(chan.getChannelNumber()>31) continue;  //ignore non-PMT channels

    for(auto const& window : chan.getWindows()) {

      uint64_t window_time = window.header().getFrame() * frame_size + window.header().getSample();

      // Exactly same timing
      if(window_time == target_time) {
	if(window.data().size() < beam_window_size) {
	  std::cout << "Unexpected readout window (same timing as beamgate but too short)" << std::endl;
	  return ub_FEMBeamTriggerOutput();
	}
	my_waveforms.emplace_back(window.data().begin()+i_begin,
				  window.data().begin()+i_end);	
	break;
      }
       
      // Concatinated waveform
      if( target_time > window_time && 
	  target_time < (window_time + window.data().size()) ) {

        std::cout << "window_time: " <<	window_time << ", window_size: " << window.data().size() << std::endl;
	my_waveforms.emplace_back(window.data().begin() + i_begin + (target_time - window_time),
				  window.data().begin() + i_end   + (target_time - window_time) );
	break;
      }
    }
  }

  if(my_waveforms.size() != 32) {
    throw datatypes_exception("Lacking some channel...");
  }

  return trig_thresh_val(my_waveforms);

  //return max_value;

}
*/
/*uint32_t ub_PMT_CardData_v6::trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave, uint32_t ThreshVal) const noexcept{

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

  }*/
/*
ub_FEMBeamTriggerOutput ub_PMT_CardData_v6::trig_thresh_val(std::vector< std::vector<uint16_t> > const& Wave) const noexcept{
  size_t diff_val0 = 3;
  size_t diff_val3 = 3;
  std::vector<short> diffvec0(Wave.at(0).size(),0); //define the size here...should it be short???
  std::vector<short> diffvec3(Wave.at(0).size(),0);
  std::vector<std::vector<short>> _chdiff(Wave.size(),std::vector<short>(Wave.at(0).size())); //need to initailize with the size._chdiff.size()==NChannel
  std::vector<std::vector<short>> _chhit(Wave.size(),std::vector<short>(Wave.at(0).size())); //initialize the vector with size.
  std::vector<short> _chdiff_sum(Wave.at(0).size()); //size of this vector is wfmsize, sum of the max from all the channels
  std::vector<short> _chhit_sum(Wave.at(0).size()); //size of this vector is wfmsize, total number of hits from PMT in the trigger window
  std::vector<short> winstarts;
  size_t disc3_min_tick = 1e9;
  size_t disc3_max_tick = 0;
  winstarts.push_back( fTriggerWinStartTick );
  //set the disc3 window when _cfg.fSetTriggerWindow is true in Matt's code.
  // define location of trigger window
  
  disc3_min_tick = fTriggerWinStartTick;
  disc3_max_tick = fWindowSize + disc3_min_tick;
  
  for (size_t i=0 ; i<Wave.size(); ++i) {//number of channels
    //std::cout<< "i: "<< i << " / " << Wave.size()-1 << std::endl;
    
    // define the diff vector 0 and 3
    
    for (size_t j=0; j+diff_val0<Wave[i].size(); ++j) {
      
      if((Wave[i][j]&0xfff) > (Wave[i][j+diff_val0]&0xfff))
	continue;
      diffvec0[j] = std::max((Wave[i][j+diff_val0]&0xfff) - (Wave[i][j]&0xfff),0);
    }
    for (size_t j=0; j+diff_val3<Wave[i].size(); ++j) {
      if((Wave[i][j]&0xfff) > (Wave[i][j+diff_val3]&0xfff))
	continue;
      diffvec3[j] = std::max((Wave[i][j+diff_val3]&0xfff) - (Wave[i][j]&0xfff),0);
    }

    // ***********************
    std::vector<short> tgth0;
    std::vector<short> ttrig0;
    std::vector<short> ttrig3;
    for (short tick=0; (tick+1)<(short)Wave[i].size(); tick++) {
      // define the trigger
      //discriminator 0 
      if ( diffvec0[tick+1]>=fDiscr0threshold && diffvec0[tick]<fDiscr0threshold ) {
	if ( ( tgth0.size()==0 || tgth0.back() + fDiscr0precount < tick+1 ) &&
	     ( ttrig3.size()==0 || ttrig3.back() + fDiscr3deadtime < tick+1 )
	     ) {
	  // form discr0 trigger
	  ttrig0.push_back( tick+1 );
	  //std::cout << "[fememu::emulate] ttrig0 @ tick " << tick+1 << std::endl;
	}
	tgth0.push_back( tick+1 );
	//std::cout << "[fememu::emulate] tgth0 @ tick " << tick+1 << std::endl;
      } // end of if discr0 fires
      
      //decriminator 3
      // There is no concept of a Discr0deatime in the firmware
      // discr3 fire
      // I am not satisfied with all the corrections in this block of code that deal with the fact
      // that the diff3 vector is only valid up to diff3.size()-_cfg.fDiscr3width. Also, I don't
      // see why _cchhit shouldn't always have the same width pulse regardless of whether it was 
      // triggered near the end of a waveform or not.
      // I consider the following only valid if the beam window is far enough from the edge of the beam
      // gate, if the Discr3width is not too big, and if Discr0deadtime == 0.
      if ( diffvec3[tick+1]>=fDiscr3threshold && diffvec3[tick]<fDiscr3threshold ) {

	// must be within discr0 prewindow and outside of past discr3 deadtime and inside beam spill window(s)
	if ( ( !ttrig0.empty() && tick+1 < fDiscr0deadtime + ttrig0.back() ) &&
	     (  ttrig3.empty() || ttrig3.back() + fDiscr3deadtime < tick+1 ) &&
	     ( tick+1 >= (short)disc3_min_tick && tick+1 <= (short)disc3_max_tick )
	     ) {
	  ttrig3.push_back( tick+1 );
	  //std::cout << "[fememu::emulate] ttrig3 @ tick " << tick+1 << std::endl;
	  // // find maxdiff
	  short tmaxdiff = diffvec3[ttrig0.back()];//diff3[tick+1];
	  short tend1 = std::min( (short)(ttrig0.back()+fDiscr3width), (short)(diffvec3.size()-1));
	  for (short t=ttrig0.back(); t<tend1; t++) {
	    if(diffvec3[t]>=diffvec3[t-1]) {
	      tmaxdiff = diffvec3[t];
	    }
	    _chdiff[i][ t+1 ] = tmaxdiff;
	    if(t-ttrig0.back()>=3) {
	      _chhit[i][ t ] = 1;
	    }
	    
	  }
	  //std::cout << "[fememu::emulate] tmax discr 3 fire: " << tmaxdiff << " " << diffvec3.at(tick+1) << " " << tick+1 << std::endl;
	}
      }// ****************************************
    }//end of wfm loop for trigger and accumulators
    //if(!ttrig3.empty())
      //std::cout << "[fememu::emulate] found disc3 " << ttrig3.size() << " fires for "  << i << std::endl;
  }//end of i loop, which is the channel loop
 

  //fill sum vector

  for(size_t tick=0; tick<Wave.at(0).size(); ++tick) { //this is originally wfmsize, which I think is the varible of the total number ticks, so I use ch0
    short phmax_sum = 0;
    short mult_sum  = 0;
    
    for(size_t ch=0; ch<Wave.size(); ++ch) { 
      phmax_sum += _chdiff[ch][tick];
      mult_sum  += _chhit[ch][tick];
    }
    _chdiff_sum[tick] = phmax_sum;
    _chhit_sum[tick]  = mult_sum;
  }

  // Prepare output (initialize in a way less change in mem seg)
  ub_FEMBeamTriggerOutput result;

  //fill trigger output
  result.window_start = fTriggerWinStartTick;
  result.window_end = result.window_start + fWindowSize + fDiscr3width;

  // ***what should I do here??
  if ( (size_t)winend>=Wave.at(0).size()) {
    std::cout << "[fememu::emulate] wiend>=wfmsize\n\n\n\n\n" << std::endl;	
    continue;
    }*********** //

  short winmaxmulti = 0;
  short winmaxdiff  = 0;
  int   fire_time   = -1;

  for (size_t tick=result.window_start; tick<=result.window_end; tick++) {
    auto const& maxdiff_ = _chdiff_sum[tick];
    auto const& nhit_ = _chhit_sum[tick];
    
    if ( winmaxdiff < maxdiff_ )
      winmaxdiff = maxdiff_;
    if ( winmaxmulti < nhit_ )
      winmaxmulti = nhit_;
    if(fire_time < 0 &&
       maxdiff_  >= fTriggerThresPHMAX &&
       nhit_     >= fTriggerThresMult)
      {fire_time = tick; // fire time should be related to trig0 firing.
	
	std::cout << "    "
			  << "@ tick "   << tick     << "  "
			  << "mult = "   << nhit_    << "  "
			  << "phmax = "  << maxdiff_ << "  "
			  << std::endl;}
  }



  // store results
  result.maxdiff = winmaxdiff;
  result.maxhit  = winmaxmulti;
  result.fire_time = fire_time;
  //std::cout << " maxdiff=" << winmaxdiff << " maxhit=" << winmaxmulti << std::endl;
  //if(fire_time >= 0)
  //std::cout << "[fememu::emulate] Trigger fired @ " << fire_time << " !" << std::endl;
  
  return result;
}
*/   
