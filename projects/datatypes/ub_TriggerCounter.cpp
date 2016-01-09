#include "ub_TriggerCounter.h"

using namespace gov::fnal::uboone::datatypes;

ub_TriggerCounter::ub_TriggerCounter()
  :_n_total(0)
{
  this->reset();
}

void ub_TriggerCounter::reset() {
  _n_total=0;
  _tc.pmt_beam=0;
  _tc.pmt_cosmic=0;
  _tc.pc=0;
  _tc.ext=0;
  _tc.active=0;
  _tc.gate2=0;
  _tc.gate1=0;
  _tc.veto=0;
  _tc.calib=0;
  _tc.gatefake=0;
  _tc.beamfake=0;
  _tc.spare1=0;
}

void ub_TriggerCounter::increment(ub_Trigger_Data_v6 const& td, bool same_total) {
  if(!same_total)          _n_total++;
  if(td.Trig_PMTBeam())    _tc.pmt_beam++;
  if(td.Trig_PMTCosmic())  _tc.pmt_cosmic++;
  if(td.Trig_PC())         _tc.pc++;
  if(td.Trig_EXT())        _tc.ext++;
  if(td.Trig_Active())     _tc.active++;
  if(td.Trig_Gate2())      _tc.gate2++;
  if(td.Trig_Gate1())      _tc.gate1++;
  if(td.Trig_Veto())       _tc.veto++;
  if(td.Trig_Calib())      _tc.calib++;
  if(td.Trig_GateFake())   _tc.gatefake++;
  if(td.Trig_BeamFake())   _tc.beamfake++;
  if(td.Trig_Spare1())     _tc.spare1++;
}

bool ub_TriggerCounter::prescalePass( std::map< uint16_t, float > const& ps, uint16_t trig_value, double random ) {
    auto iThr = ps.lower_bound( trig_value );
    if ( iThr == ps.end() ) return true;
    else if ( random < iThr->second ) return true;

    return false;
}

std::string ub_TriggerCounter::debugInfo()const noexcept
{
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << ".";
    os << "\n Total triggers= "       << get_N_Total();
    os << "\n   PMTBeam triggers= "   << get_N_PMT_Beam();
    os << "\n   PMTCosmic triggers= " << get_N_PMT_Cosmic();
    os << "\n   PC triggers= "        << get_N_PC();
    os << "\n   EXT triggers= "       << get_N_EXT();
    os << "\n   Active triggers= "    << get_N_Active();
    os << "\n   Gate2 triggers= "     << get_N_Gate2();
    os << "\n   Gate1 triggers= "     << get_N_Gate1();
    os << "\n   Veto triggers= "      << get_N_Veto();
    os << "\n   Calib triggers= "     << get_N_Calib();
    os << "\n   GateFake triggers= "  << get_N_GateFake();
    os << "\n   BeamFake triggers= "  << get_N_BeamFake();
    os << "\n   Spare1 triggers= "    << get_N_Spare1();

    return os.str();
}
