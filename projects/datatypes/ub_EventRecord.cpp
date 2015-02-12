#include "ub_EventRecord.h"

using namespace gov::fnal::uboone::datatypes;


void ub_EventRecord::addFragment(raw_data_containter<raw_data_type>& fragment)
{  
   artdaq_fragment_header const* artdaq_header= reinterpret_cast<artdaq_fragment_header const*>(&* fragment.begin());
  
   if(artdaq_fragment_header::num_words()<=fragment.size())
	throw datatypes_exception("Invalid fragment: fragment is too short.");      
	
  ub_CrateHeader_v6 const & crate_header= ub_CrateHeader_v6::getHeaderFromFragment(fragment);
  
  if(crate_header.crate_type == 2)
  {
    _pmt_seb_map.emplace(crate_header.crate_number,std::make_tuple(
	std::move(fragment),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<pmt_crate_data_t>(nullptr)));
	
     raw_data_containter<raw_data_type>& tpm_fragment=std::get<0>(_pmt_seb_map[crate_header.crate_number]);
     auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
					  artdaq_fragment_header::num_words(),tpm_fragment.end());
     std::get<1>(_pmt_seb_map[crate_header.crate_number]).swap(raw_data);     
     auto crate_data = std::make_unique<pmt_crate_data_t>(*std::get<1>(_pmt_seb_map[crate_header.crate_number]));
     auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
     crate_data->crateHeader().swap(header);
     std::get<2>(_pmt_seb_map[crate_header.crate_number]).swap(crate_data);
  }
  else
  {
    _tpc_seb_map.emplace(crate_header.crate_number,std::make_tuple(
	std::move(fragment),std::unique_ptr<ub_RawData>(nullptr),std::unique_ptr<tpc_crate_data_t>(nullptr)));

     raw_data_containter<raw_data_type>& tpm_fragment=std::get<0>(_tpc_seb_map[crate_header.crate_number]);
     auto raw_data = std::make_unique<ub_RawData>(tpm_fragment.begin()+artdaq_header->metadata_word_count+
					  artdaq_fragment_header::num_words(),tpm_fragment.end());
     std::get<1>(_tpc_seb_map[crate_header.crate_number]).swap(raw_data); 
     auto crate_data = std::make_unique<tpc_crate_data_t>(*std::get<1>(_tpc_seb_map[crate_header.crate_number]));     
     auto header=std::make_unique<ub_CrateHeader_v6>(crate_header);
     std::get<2>(_tpc_seb_map[crate_header.crate_number]).swap(crate_data);
  }

  getGlobalHeader().setNumberOfBytesInRecord(getGlobalHeader().getNumberOfBytesInRecord()+crate_header.size);
  getGlobalHeader().setNumberOfSEBs((uint8_t)(_tpc_seb_map.size() + _pmt_seb_map.size()));    
}

const ub_EventRecord::tpc_map_t ub_EventRecord::getTPCSEBMap() const
{
  tpc_map_t retMap;
  for(auto& tpc : _tpc_seb_map)
    retMap.emplace(tpc.first,*std::get<2>(tpc.second));
  return retMap;
}
const ub_EventRecord::pmt_map_t ub_EventRecord::getPMTSEBMap() const
{
  pmt_map_t retMap;
  for(auto& pmt : _pmt_seb_map)
    retMap.emplace(pmt.first,*std::get<2>(pmt.second));
  return retMap;
}

void ub_EventRecord::getFragments(fragment_references_t& fragments) const
{
  for(auto& tpc : _tpc_seb_map)
      fragments.emplace_back(&std::get<0>(tpc.second));
  for(auto& pmt : _pmt_seb_map)
      fragments.emplace_back(&std::get<0>(pmt.second));
}