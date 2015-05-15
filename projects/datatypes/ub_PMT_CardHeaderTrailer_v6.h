#ifndef _UBOONE_TYPES_PMTCARDHEADERTRAILER_V6_H
#define _UBOONE_TYPES_PMTCARDHEADERTRAILER_V6_H 1

#include "uboone_data_utils.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

/**Card/Module Header format: (each module sends a header followed by adc data)
first word  {16'hffff,               4'h7,id[6:0],module[4:0]}
2nd word    {4'h7,eventnumber[23:12], 4'h7,eventnumber[11:0]}
3d word     {4'h7,wordcount[23:12],  4'h7,wordcount[11:0]}
4th word    {4'h7,checksum[23:12],   4'h7,checksum[11:0]}
5th word    {16'h4000}
**/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
struct ub_PMT_CardHeader_v6 final
{
    union {
        struct {
            uint32_t mark 	:16;
            uint32_t module	:5;
            uint32_t id  	:7;
            uint32_t  		:4;
        };
        uint32_t id_and_module =0xDEADBEEF;
    } __attribute__ ((packed));
    uint8_t getID()const noexcept {
        return (uint8_t)id;
    }
    uint8_t getModule()const noexcept {
        return (uint8_t) module;
    }
    uint16_t getMark()const noexcept {
        return (uint16_t)mark;
    }

    union {
        struct {
            uint32_t wc2  		:12;
            uint32_t  			:4;
            uint32_t wc1  		:12;
            uint32_t       		:4;
        };
        uint32_t word_count=0xDEADBEEF;
    } __attribute__ ((packed));
    uint32_t getWordCount()const noexcept {
      return (wc2<<12|(wc1 +1))-2;//subtract two for the 4000 and c000
    }

    union {
        struct {
            uint32_t en2  		:12;
            uint32_t  			:4;
            uint32_t en1  		:12;
            uint32_t       		:4;
        };
        uint32_t event_number=0xDEADBEEF;
    } __attribute__ ((packed));
    uint32_t getEvent()const noexcept {
        return en2<<12|en1;
    }

    union {
        struct {
            uint32_t fn2  		:12;
            uint32_t  			:4;
            uint32_t fn1  		:12;
            uint32_t       		:4;
        };
        uint32_t frame_number=0xDEADBEEF;
    } __attribute__ ((packed));
    uint32_t getFrame()const noexcept {
        return fn2<<12|fn1;
    }


    union {
        struct {
            uint32_t cs2  		:12;
            uint32_t  			:4;
            uint32_t cs1  		:12;
            uint32_t       		:4;
        };
        uint32_t checksum=0xDEADBEEF;
    } __attribute__ ((packed));
    uint32_t getChecksum()const noexcept {
        return cs2<<12|cs1;
    }

    union {
        struct  {
            uint32_t tf2  		:4;
            uint32_t tf3 		:4;
            uint32_t  			:4;
            uint32_t  			:4;
            uint32_t tf1  		:8;
            uint32_t       		:4;
        };
        uint32_t trig_frame_and_sample=0xDEADBEEF;
    } __attribute__ ((packed));
    uint32_t getTrigSample()const noexcept {
        return tf2<<8|tf1;
    }
    uint32_t getTrigFrameMod16()const noexcept {
        return tf3;
    }
    
    union {
      struct  {
	uint16_t data_start_marker;
      };
      uint16_t raw_data_start_marker=0xDEAD;
    } __attribute__ ((packed));
    uint16_t getDataStartMarker() const noexcept{
      return data_start_marker;
    }
    
    std::string debugInfo() const noexcept;
} __attribute__ ((packed));

struct ub_PMT_CardTrailer_v6 final
{
    union {
      struct  {
	uint16_t data_end_marker;
      };
      uint16_t raw_data_end_marker=0xBEEF;
    };
    uint16_t getDataEndMarker() const noexcept{
      return data_end_marker;
    }

    std::string debugInfo() const noexcept;
};

static_assert (sizeof(ub_PMT_CardHeader_v6) == 26, "ub_PMT_CardHeader_v6 structure size is not correct.");
static_assert (sizeof(ub_PMT_CardTrailer_v6) == 2, "ub_PMT_CardTrailer_v6 structure size is not correct.");

#pragma GCC diagnostic pop

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov


#endif //_UBOONE_TYPES_PMTCARDHEADERTRAILER_V6_H
