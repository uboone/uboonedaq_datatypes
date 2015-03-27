#ifndef _UBOONETYPES_EVENTRECORD_H
#define _UBOONETYPES_EVENTRECORD_H


#include <assert.h>
#include "evttypes.h"
#include "constants.h"
#include "boostSerialization.h"
#include "raw_data_access.h"


/***
    The ub_EventRecord is meant to house all of the components of the
    final data format as it leaves the assembler and is written to
    disk. The data will be written as a boost binary_archive, so
    we may version different pieces accordingly. Along with all the
    (independently) serialized headers, we have a map that pairs
    crate headers and crate data.
 ***/


namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

using namespace gov::fnal::uboone::datatypes;

class ub_EventRecord {

public:
    typedef raw_data_containter<raw_data_type>   raw_fragment_data_t;
    typedef raw_fragment_data_t::value_type      fragment_value_type_t;
    
    typedef std::tuple<raw_fragment_data_t,
            std::unique_ptr<ub_RawData>,
            std::unique_ptr<tpc_crate_data_t>> tpc_crate_data_tuple_t;
    typedef std::map<int,tpc_crate_data_tuple_t> tpc_seb_map_t;

    typedef std::map<int,tpc_crate_data_t const&> tpc_map_t;

    typedef std::tuple<raw_fragment_data_t,
            std::unique_ptr<ub_RawData>,
            std::unique_ptr<pmt_crate_data_t>> pmt_crate_data_tuple_t;
    typedef std::map<int,pmt_crate_data_tuple_t> pmt_seb_map_t;

    typedef std::map<int,pmt_crate_data_t const&> pmt_map_t;

    typedef std::vector<raw_fragment_data_t const*> fragment_references_t;

    static const uint8_t DAQ_version_number = gov::fnal::uboone::datatypes::constants::VERSION;
    ub_EventRecord():_global_header(),_tpc_seb_map(),_pmt_seb_map() {}
    void setGlobalHeader (global_header_t & header) {
        _global_header = header;
    }
    global_header_t& getGlobalHeader() throw() {
        return _global_header;
    }

    void addFragment(raw_fragment_data_t & fragment) throw(datatypes_exception);

    bool compare(const ub_EventRecord& event_record, bool do_rethrow) const throw(datatypes_exception);

    const tpc_map_t getTPCSEBMap() const throw(datatypes_exception);
    const pmt_map_t getPMTSEBMap() const throw(datatypes_exception);
    void updateDTHeader();
    
    void  getFragments(fragment_references_t& fragments) const throw(datatypes_exception);

//  void setTriggerData (triggerData tD) { trigger_data = tD; }
//  void setGPS (ub_GPS g) { gps_data = g; }
//  void setBeamHeader (beamHeader bH) { beam_header = bH; }
//  void insertBeamData (beamData bD) { beam_data_vector.push_back(bD); }
//  ub_GlobalHeader getGlobalHeader() { return global_header; }
//  triggerData getTriggerData() { return trigger_data; }
//  ub_GPS getGPS() { return gps_data; }
//  beamHeader getBeamHeader() { return beam_header; }
//  std::vector<beamData> getBeamDataVector() { return beam_data_vector; }
//  triggerData* getTriggerDataPtr() { return &trigger_data; }
//  ub_GPS* getGPSPtr() { return &gps_data; }
//  beamHeader* getBeamHeaderPtr() { return &beam_header; }
//  int getBeamDataVecotr_size() { return beam_data_vector.size(); }
//  void clearBeamDataVector() { beam_data_vector.clear(); }
private:
    ub_event_header    _bookkeeping_header;
    ub_event_trailer   _bookkeeping_trailer;
    global_header_t    _global_header;
    tpc_seb_map_t      _tpc_seb_map;
    pmt_seb_map_t      _pmt_seb_map;

//  triggerData trigger_data;
//  ub_GPS gps_data;
//  beamHeader beam_header;
//  std::vector<beamData> beam_data_vector;
//  uint8_t er_IO_mode;

    #define UNUSED(x) (void)(x)
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {	
	UNUSED(version);
	
        //BEGIN SERIALIZE RAW EVENT FRAGMENT DATA
        fragment_references_t fragments;
        getFragments(fragments);
	assert(_bookkeeping_header.event_fragment_count==fragments.size());
	assert(_bookkeeping_header.raw_event_fragments_wordcount==std::accumulate(
	  fragments.begin(),fragments.end(),0u,[](auto total, auto const& fragment){
	      return total+fragment->size()*sizeof(fragment_value_type_t);}));

	 // write bookkeeping info          
	 ar.save_binary(&_bookkeeping_header,ub_event_header_size);
	 
	// write raw fragmetns with crate headers        
        for(auto const& fragment : fragments){
            std::size_t size{fragment->size()};
            ar.save_binary(&size, sizeof(std::size_t)) ;
            ar.save_binary(fragment->data(),size*sizeof(fragment_value_type_t));
        }
	//END SERIALIZE RAW EVENT FRAGMENT DATA
#if 0	
        // write remaining event details
        if(version>=3)
            ar //& er_IO_mode
            & _global_header
            //& trigger_data
            //& gps_data
            //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
            ;

        else if(version>1)
            ar //& er_IO_mode
            & _global_header
            //& trigger_data
            //& gps_data
            //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
            ;
        else if(version>0)
            ar //& er_IO_mode
            & _global_header
            ;
#endif 
        //this must be the last step
        ar.save_binary(&_bookkeeping_trailer,ub_event_trailer_size);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        UNUSED(version);
        //BEGIN SERIALIZE RAW EVENT FRAGMENT DATA
        // read bookkeeping info          
        ar.load_binary(&_bookkeeping_header,ub_event_header_size);
        assert(_bookkeeping_header.mark_E974==UBOONE_EHDR); 
	// write raw fragmetns with crate headers        
        for(std::size_t frag_number=0; frag_number < _bookkeeping_header.event_fragment_count; frag_number++)
        {
            raw_fragment_data_t fragment;         
	    std::size_t size;
            ar.load_binary(&size,sizeof(std::size_t)) ;
	    fragment.resize(size);
            ar.load_binary(fragment.data(),fragment.size()*sizeof(fragment_value_type_t));
            addFragment(fragment);
        }
        fragment_references_t fragments;
        getFragments(fragments);        
	assert(_bookkeeping_header.raw_event_fragments_wordcount==std::accumulate(
	  fragments.begin(),fragments.end(),0u,[](auto total, auto const& fragment){
	      return total+fragment->size()*sizeof(fragment_value_type_t);}));        
	//END SERIALIZE RAW EVENT FRAGMENT DATA

#if 0        
        if(version>=3)
            ar //& er_IO_mode
            & _global_header
            //& trigger_data
            //& gps_data
            //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
            ;

        else if(version>1)
            ar //& er_IO_mode
            & _global_header
            //& trigger_data
            //& gps_data
            //& beam_header & beam_data_vector //beam stuff...empty at first, added in later
            ;
        else if(version>0)
            ar //& er_IO_mode
            & _global_header
            ;
#endif
        //this must be the last step
        ar.load_binary(&_bookkeeping_trailer,ub_event_trailer_size);
        assert(_bookkeeping_trailer.mark_974E==UBOONE_ETLR);            
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}  // end of namespace gov

// This MACRO must be outside any namespaces.
BOOST_CLASS_VERSION(gov::fnal::uboone::datatypes::ub_EventRecord, gov::fnal::uboone::datatypes::constants::VERSION)

#endif /* #ifndef BOONETYPES_H */
