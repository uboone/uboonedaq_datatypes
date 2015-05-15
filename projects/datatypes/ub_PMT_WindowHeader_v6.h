#ifndef _UBOONE_TYPES_PMTWINDOWHEADER_H
#define _UBOONE_TYPES_PMTWINDOWHEADER_H 1

#include "uboone_data_utils.h"
#include <map>
#include <cstring>
#include <string>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {
/** PMT WINDOW Control Words.
    3 16-bit words.
    (1) Contains channel number and discriminator type fired.
    (2) Contains Frame number and part of sample number in which 
        trigger was fired.
    (3) Contains rest of sample number when trigger was fired.
**/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

enum ub_PMT_DiscriminatorTypes_v6 {
  COSMIC=0,
  MICHEL,
  BEAM,
  BEAM_GATE
};

const std::map<int,std::string> ub_PMT_DiscriminatorType_v6ToString
= { { ub_PMT_DiscriminatorTypes_v6::COSMIC, "COSMIC" },
    { ub_PMT_DiscriminatorTypes_v6::MICHEL, "MICHEL" },
    { ub_PMT_DiscriminatorTypes_v6::BEAM, "BEAM" },
    { ub_PMT_DiscriminatorTypes_v6::BEAM_GATE, "BEAM_GATE" } };

struct ub_PMT_WindowHeader_v6 final
{
    //first word
    union {
        struct {
            uint16_t channel_number :6;
	    uint16_t empty1         :4;
            uint16_t discriminator  :2;
	    uint16_t marker1        :2;
	    uint16_t                :2;
        } ;
        uint16_t ch_and_disc=0xDEAD;
    } ;
    uint8_t getChannelNumber() const noexcept{
        return channel_number;
    }
    uint8_t getDiscriminantor() const noexcept{
        return discriminator;
    }
    std::string getDiscriminatorType() const noexcept{
      return ub_PMT_DiscriminatorType_v6ToString.at(discriminator);
    }
    uint8_t getMarker1() const noexcept{
      return marker1;
    }

    //second word
    union {
        struct {
            uint16_t s1             :5;
	    uint16_t frame          :3;
            uint16_t empty2         :4;
	    uint16_t marker2        :2;
	    uint16_t                :2;
        } ;
        uint16_t frame_and_sample1=0xDEAD;
    } ;
    uint16_t getFrame() const noexcept{
      return frame;
    }
    uint8_t getMarker2() const noexcept{
      return marker2;
    }

    //third word
    union {
        struct {
            uint16_t s2             :12;
	    uint16_t marker3        :2;
	    uint16_t                :2;
        } ;
        uint16_t sample2=0xDEAD;
    } ;
    uint32_t getSample() const noexcept{
      return ( (s1<<12)+s2 );
    }
    uint8_t getMarker3() const noexcept{
      return marker3;
    }

    std::string debugInfo()const noexcept;
} ;
static_assert (sizeof(ub_PMT_WindowHeader_v6) == 6, "ub_PMT_WindowHeader_v6 structure size is not correct.");

#pragma GCC diagnostic pop

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_PMTWINDOWHEADER_H
