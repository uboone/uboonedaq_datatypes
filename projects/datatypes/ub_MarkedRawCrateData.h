#ifndef _UBOONE_TYPES_MARKEDRAWCRATEDATA_H
#define _UBOONE_TYPES_MARKEDRAWCRATEDATA_H 1

#include "uboone_data_common.h"
#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_CardDataCreatorHelperClass.h"
#include "ub_LocalHostTime.h"
#include <mutex>

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename CARD, typename HEADER, typename TRAILER> class ub_MarkedRawCrateData :
  public ub_MarkedRawDataBlock<HEADER,TRAILER> {
public:
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::header_type crate_header_type;
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer_type crate_trailer_type;
    typedef CARD card_t;
    template <typename MRCD> using dissector_type = ub_CardDataCreatorHelperClass<MRCD>;

    explicit ub_MarkedRawCrateData(ub_RawData const& rawdata) 
      try
      :ub_MarkedRawDataBlock<HEADER,TRAILER>(rawdata),
        _initializeHeaderFromRawData {false},
        _markedRawCardsData {},
        _dissectableDataSize {0},
        _crateHeader {nullptr},
        _dissection_exception(""),
        _isValid {isValid()},
        _isFullyDissected {canFullyDissect()} 
        {} 
       catch(std::exception &e) {
	     std::cerr << "Caught exception ub_MarkedRawCrateData::ctor(). Message: " << e.what() << std::endl;
	     throw;
       }
       catch(...) {	
	     std::cerr << "Caught unknown exception ub_MarkedRawCrateData::ctor()" << std::endl;	     
	     throw datatypes_exception("Caught unknown exception ub_MarkedRawCrateData::ctor()");
       }

    explicit ub_MarkedRawCrateData(ub_RawData const& rawdata,bool initializeHeaderFromRawData)
      try
      :ub_MarkedRawDataBlock<HEADER,TRAILER>(rawdata),
        _initializeHeaderFromRawData {initializeHeaderFromRawData},
        _markedRawCardsData {},
        _dissectableDataSize {0},
        _crateHeader {nullptr},
        _dissection_exception(""),
        _isValid {
        isValid()
        },
        _isFullyDissected {
        canFullyDissect()
        } {}
       catch(std::exception &e) {
	     std::cerr << "Caught exception ub_MarkedRawCrateData::ctor(). Message: " << e.what() << std::endl;
	     throw;
       }
       catch(...) {
	     std::cerr << "Caught unknown exception ub_MarkedRawCrateData::ctor()" << std::endl;
	     throw datatypes_exception("Caught unknown exception ub_MarkedRawCrateData::ctor()");
       }
        
    std::vector<CARD> const&  getCards() throw(data_size_exception,datatypes_exception);
    std::vector<CARD> const&  getCards() const noexcept{
        return _markedRawCardsData;
    }

    ub_MarkedRawCrateData() = delete;
    ub_MarkedRawCrateData ( ub_MarkedRawCrateData const& ) = delete;
    ub_MarkedRawCrateData ( ub_MarkedRawCrateData && ) = default;
    ub_MarkedRawCrateData& operator= ( ub_MarkedRawCrateData const& ) = delete;
    ub_MarkedRawCrateData& operator= ( ub_MarkedRawCrateData && ) = delete;
    
    ~ub_MarkedRawCrateData(){_crateHeader.reset(nullptr); _markedRawCardsData.clear();}

    size_t getSizeOfCardsData() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::data().size();
    };
    size_t getSizeOfRAWCrateData() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::rawdata().size();
    };

    void dissectCards() throw(data_size_exception,datatypes_exception);
    std::string debugInfo()const noexcept;

    size_t getSizeOfDissectableCrateData() const noexcept{
        assert(_dissectableDataSize !=0 );        
        return _dissectableDataSize;
    };

    std::unique_ptr<typename CARD::ub_CrateHeader>& crateHeader() throw(data_size_exception,datatypes_exception);
    std::unique_ptr<typename CARD::ub_CrateHeader> const& crateHeader()const noexcept{
        assert(_crateHeader);
        return _crateHeader;
    };

    bool compare(ub_MarkedRawCrateData const&,bool do_rethrow=false) const throw(datatypes_exception);
    
    static void         doDissect(bool dod)          { _do_dissect = dod ; } // Allow user to turn off unpacking.
    bool                wasDissected() const { return _isFullyDissected; }
    datatypes_exception dissectionException() const { return _dissection_exception; }

    void rethrowDissectionException() const throw(data_size_exception,datatypes_exception);
    
private:
    bool isValid() noexcept;
    bool canFullyDissect() noexcept;   
    void reportMissingTrailer() noexcept;   
    std::once_flag flagfemcarderr;
private:
    static bool  _do_dissect;   
    bool _initializeHeaderFromRawData;
    std::vector<CARD> _markedRawCardsData;
    size_t _dissectableDataSize;
    std::unique_ptr<typename CARD::ub_CrateHeader> _crateHeader;
    datatypes_exception _dissection_exception;
    mutable bool _isValid;
    bool _isFullyDissected;        
};

template <typename CARD, typename HEADER, typename TRAILER>
std::vector<CARD> const&  ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::getCards() throw(data_size_exception,datatypes_exception)
{
    if(!_isFullyDissected && _isValid)
        dissectCards();

    return _markedRawCardsData;
}

template <typename CARD, typename HEADER, typename TRAILER>
void ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::dissectCards() throw(data_size_exception,datatypes_exception)
{
  std::call_once(flagfemcarderr, [](){ganglia::Metric<ganglia::RATE>::named("FEM-card-dissection-errors","Errors/sec")->publish(0);});

    try
    {
	_isValid=false; //reset the isValid flag

	if ( handle_missing_words<CARD>() ){

	  ub_RawData data_plus_trailer{ub_MarkedRawDataBlock<HEADER,TRAILER>::data().begin(),
	      ub_MarkedRawDataBlock<HEADER,TRAILER>::rawdata().end()};
	  
	  dissector_type<CARD> dissector(data_plus_trailer);
	  dissector.populateCardDataVector(_markedRawCardsData);         	  
	  _dissectableDataSize= size_of<crate_header_type>()+dissector.getTrueDataSize();
	}
	else{
	  dissector_type<CARD> dissector(ub_MarkedRawDataBlock<HEADER,TRAILER>::data());
	  dissector.populateCardDataVector(_markedRawCardsData);         	  
	  _dissectableDataSize= ub_MarkedRawDataBlock<HEADER,TRAILER>::minsize()+dissector.getTrueDataSize();
	}
	  assert(_dissectableDataSize > (ub_MarkedRawDataBlock<HEADER,TRAILER>::minsize()));
	  //assert(_dissectableDataSize <=  (ub_MarkedRawDataBlock<HEADER,TRAILER>::rawdata().size()));
	  assert(_markedRawCardsData.size()>0);
        
        _isFullyDissected=true;
        
         auto fem_dissection_errors=ganglia::RATE<void>::preferred_type{0};                 
         for(auto & card: _markedRawCardsData){
	    if(!card.isValid())	++fem_dissection_errors;
	  }
	 ganglia::Metric<ganglia::RATE>::named("FEM-card-dissection-errors","Errors/sec")->publish(fem_dissection_errors);
	 
	 reportMissingTrailer();   

        _isValid=true;
       // std::cerr << ub_data_types::debugInfoShort(ub_RawData{rawdata().begin(),rawdata().begin()+_dissectableDataSize}) <<std::endl;
    }
    // If there's a problem unpacking card-level data, sets _fully to false and stores the exception.
    catch(data_size_exception &ex){
      throw;
    }catch(datatypes_exception &ex){
      _dissection_exception = ex;      
      throw;
    }catch(std::exception &e){
          datatypes_exception de(std::string("Caught std::exception in ub_MarkedRawCrateData::dissectCards(). Message:").append(e.what()));
          _dissection_exception = de;
          throw de;                
    }catch(...){
        datatypes_exception de("Caught unknown exception in ub_MarkedRawCrateData::dissectCards().");
        _dissection_exception = de;
        throw de;                
    }
}

template <typename CARD, typename HEADER, typename TRAILER>
bool ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::canFullyDissect() noexcept
{
    try
    { 
        if(! ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::_do_dissect)
	  return false;
	  
	if(!_isValid){
	  _dissection_exception = datatypes_exception("ub_MarkedRawCrateData was marked as invalid.");
	  return false;
	}
	_isFullyDissected=false;	
        dissectCards();        
        crateHeader();
    } catch(std::exception &ex) {
        std::cerr << "Exception:" << ex.what() << std::endl;
        _isValid=false;

	std::cerr << debugInfo() << std::endl;

        return false;
    }catch(...){
        std::cerr << "Caught unknown exception in ub_MarkedRawCrateData::canFullyDiessect().";

	std::cerr << debugInfo() << std::endl;

        _isValid=false;
        return false;
    }
    return true;
}

template <typename CARD, typename HEADER, typename TRAILER>
std::unique_ptr<typename CARD::ub_CrateHeader>& ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::crateHeader() throw(data_size_exception,datatypes_exception)
{
  try{
    if(_crateHeader)
        return _crateHeader;

    std::unique_ptr<typename CARD::ub_CrateHeader> crateHeader{nullptr};
    
    if(_initializeHeaderFromRawData && _isValid){
         assert(!getCards().empty());           
         crateHeader.reset(new typename CARD::ub_CrateHeader(getCards().begin()->header()));
         crateHeader->card_count=getCards().size();
         crateHeader->size=_dissectableDataSize;
         crateHeader->crate_type=stringToSystemDesignator.at(CARD::typeName);
         HasLocalHostTime().update().copyOut(crateHeader->local_host_time);
         crateHeader->updateCrateBits();
         for(auto const& card: getCards()){
	  if(card.isValidChecksum())
	    continue;
	    
	    crateHeader->data_transmission_header.flagChecksumAsInvalid();
	    crateHeader->complete=false;
	    
	    break;
	 }
    } else {
         crateHeader.reset(new typename CARD::ub_CrateHeader());
    }

    assert(crateHeader);
        
    _crateHeader.swap(crateHeader);
    assert(_crateHeader);
    return _crateHeader;
  }
  catch(datatypes_exception &ex) {
    std::cerr << ex.what();
    throw;
  } catch(...) {
    std::cerr << "Unknown exception in ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::crateHeader()";
    throw datatypes_exception("Unknown exception in ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::crateHeader()");
  }  
  
}

template <typename CARD, typename HEADER, typename TRAILER>
bool ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::compare(ub_MarkedRawCrateData<CARD,HEADER,TRAILER> const& marked_data,bool do_rethrow) 
  const throw(datatypes_exception)
{
    try
    {
        if(_isValid!=marked_data._isValid)
            throw datatypes_exception(make_compare_message("ub_MarkedRawCrateData", "_isValid", _isValid,marked_data._isValid));

        if(_crateHeader->compare(*marked_data._crateHeader))
            throw datatypes_exception(make_compare_message("ub_MarkedRawCrateData", "_crateHeader", _crateHeader->debugInfo(),marked_data._crateHeader->debugInfo()));

        if(_markedRawCardsData.size()!=marked_data._markedRawCardsData.size())
            throw datatypes_exception(make_compare_message("ub_MarkedRawCrateData._markedRawCardsData", "size", _markedRawCardsData.size(),marked_data._markedRawCardsData.size()));

        auto mismatching_data = std::mismatch(_markedRawCardsData.begin(), _markedRawCardsData.end(), marked_data._markedRawCardsData.begin());
        auto data_ends =std::make_pair(_markedRawCardsData.end(),marked_data._markedRawCardsData.end());

        if(mismatching_data!=data_ends)
            throw datatypes_exception("Error: ub_MarkedRawCrateData are not matching.");

    } catch(datatypes_exception &ex) {
        std::cerr << ex.what();
        if(do_rethrow)
            throw;
        else
            return false;
    } catch(...) {
        std::cerr << "Unknown exception in ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::compare()");
        else
            return false;
    }
    return true;
}

template <typename CARD, typename HEADER, typename TRAILER>
std::string ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::debugInfo()const noexcept
{
    std::size_t idx{0};
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::header().debugInfo();
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer().debugInfo();

    os << " *Found " << std::dec << getCards().size() << " cards." << std::endl;  
    
    for(auto const& card : getCards())
	os << "Card " << ++idx << std::endl << card.debugInfo();

    os <<  ub_MarkedRawDataBlock<HEADER,TRAILER>::debugInfo();
    return os.str();
}

template <typename CARD, typename HEADER, typename TRAILER>
void ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::rethrowDissectionException() const throw(data_size_exception,datatypes_exception)
{
  if(!_isValid)      
    throw _dissection_exception;

  std::ostringstream os;
  
  for(auto const& card :_markedRawCardsData){	
    try
    {
      card.rethrowDissectionException();
    }catch(datatypes_exception &ex){
      os << ex.what() << "\n"; _isValid=false;
    }
  }
 
 if(!_isValid)
   throw datatypes_exception(os.str());
}

template <typename CARD, typename HEADER, typename TRAILER>
void __attribute__ ((noinline))  ub_MarkedRawCrateData<CARD,HEADER,TRAILER>::reportMissingTrailer() noexcept
{
//do nothing
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCRATEDATA_H

