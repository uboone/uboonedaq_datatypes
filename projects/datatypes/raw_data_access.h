#ifndef _UBOONE_DATATYPES_RAWDATAACCESS_H
#define _UBOONE_DATATYPES_RAWDATAACCESS_H  1

#include "uboone_data_v6_headers.h"

using namespace gov::fnal::uboone::datatypes;	


typedef ub_EventTrailer event_trailer_t;
typedef ub_EventHeader event_header_t;
typedef ub_ChannelHeader channel_header_t;
typedef ub_ChannelTrailer channel_trailer_t;

typedef ub_PMT_CardHeader_v6  pmt_card_header_t;
typedef ub_TPC_CardHeader_v6  card_header_t;
typedef ub_CrateHeader crate_header_t; //FIXME:GAL uncoment
//typedef ub_EventTrailer crate_trailer_t;
#endif //_UBOONE_DATATYPES_RAWDATAACCESS_H

