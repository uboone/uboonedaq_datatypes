#ifndef _UBOONE_DATATYPES_RAWDATAACCESS_H
#define _UBOONE_DATATYPES_RAWDATAACCESS_H  1

#include "uboone_data_v6_headers.h"

using namespace gov::fnal::uboone::datatypes;

typedef ub_XMITEventTrailer event_trailer_t;
typedef ub_XMITEventHeader  event_header_t;
typedef ub_ChannelHeader channel_header_t;
typedef ub_ChannelTrailer channel_trailer_t;

typedef ub_PMT_CardHeader_v6  pmt_card_header_t;
typedef ub_TPC_CardHeader_v6  tpc_card_header_t;
typedef ub_TPC_CardHeader_v6  card_header_t;
typedef ub_PMT_CrateData_v6   pmt_crate_data_t;
typedef ub_TPC_CrateData_v6   tpc_crate_data_t;

typedef ub_TPC_CrateData_v6   crate_data_t;

typedef ub_CrateHeader_v6     crate_header_t;
typedef ub_GlobalHeader       global_header_t;


//typedef ub_XMITEventTrailer crate_trailer_t;
#endif //_UBOONE_DATATYPES_RAWDATAACCESS_H

