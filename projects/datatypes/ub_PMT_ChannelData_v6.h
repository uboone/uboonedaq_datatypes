#ifndef _UBOONE_TYPES_PMTCHANELDATA_V6_H
#define _UBOONE_TYPES_PMTCHANELDATA_V6_H 1

#include "ub_MarkedRawChannelData.h"
#include "ub_PMT_WindowData_v6.h"


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {


class ub_PMT_ChannelData_v6 final
{
public:
    ub_PMT_ChannelData_v6( int channel, std::vector<ub_PMT_WindowData_v6>& windows)
      { _channel = channel; _windows.swap(windows); } 

    ub_PMT_ChannelData_v6 ( ub_PMT_ChannelData_v6 && ) = default;
    
    
    int getChannelNumber() const { return _channel; }
    std::vector<ub_PMT_WindowData_v6> const& getWindows() const noexcept{
      return _windows;
    };
        
        
    std::string debugInfo()const noexcept {
      return std::string("  Channel ") + std::to_string(_channel) + " with " + std::to_string(_windows.size()) + " windows ";
    }
    
    
    
    static constexpr auto typeName="PMT";
    
  private:
    std::vector<ub_PMT_WindowData_v6>  _windows;
    int                                _channel;

};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCHANELDATA_V6_H
