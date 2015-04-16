#ifndef _UBOONE_TYPES_MARKEDRAWCRATEDATA_H
#define _UBOONE_TYPES_MARKEDRAWCRATEDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_XMITEventHeaderTrailer_v0.h"
#include "ub_CardDataCreatorHelperClass.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename CARD> class ub_MarkedRawCrateData :
    public ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer> {
public:
    template <typename MRCD> using dissector_type = ub_CardDataCreatorHelperClass<MRCD>;

    explicit ub_MarkedRawCrateData(ub_RawData const rawdata):
        ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer>(rawdata),
        _markedRawCardsData {},_dissectableDataSize {0},_crateHeader {nullptr},_isValid {isValid()},
    _isFullyDissected {canFullyDissect()},_createHeaderFromData {false} {}

    explicit ub_MarkedRawCrateData(ub_RawData const rawdata,bool createHeaderFromData):
        ub_MarkedRawDataBlock<ub_XMITEventHeader,ub_XMITEventTrailer>(rawdata),
        _markedRawCardsData {},_dissectableDataSize {0},_crateHeader {nullptr},_isValid {isValid()},
    _isFullyDissected {canFullyDissect()},_createHeaderFromData {createHeaderFromData} {}

    uint32_t const& getHeaderWord() const noexcept{
        return header().raw_data;
    }
    uint32_t const& getTrailerWord() const noexcept{
        return trailer().raw_data;
    }
    std::vector<CARD> const&  getCards() throw(datatypes_exception);
    std::vector<CARD> const&  getCards() const noexcept{
        return _markedRawCardsData;
    }

    ub_MarkedRawCrateData() = delete;
    //ub_MarkedRawCrateData(ub_MarkedRawCrateData const &) = delete;
    ub_MarkedRawCrateData& operator=(ub_MarkedRawCrateData const &) = delete;
    size_t getSizeOfCardsData() const noexcept{
        return data().size();
    };
    size_t getSizeOfRAWCrateData() const noexcept{
        return rawdata().size();
    };

    void dissectCards() throw(datatypes_exception);
    std::string debugInfo()const noexcept;

    size_t getSizeOfDissectableCrateData() const noexcept{
        assert(_dissectableDataSize !=0 );
        return _dissectableDataSize;
    };

    std::unique_ptr<typename CARD::ub_CrateHeader>& crateHeader() throw(datatypes_exception);
    std::unique_ptr<typename CARD::ub_CrateHeader> const& crateHeader()const noexcept{	
        return _crateHeader;
    };

    bool compare(ub_MarkedRawCrateData const&,bool do_rethrow=false) const throw(datatypes_exception);
     bool initialize() throw(datatypes_exception);
private:
    bool isValid() noexcept;
    bool canFullyDissect() noexcept;
   
    
private:
    std::vector<CARD> _markedRawCardsData;
    size_t _dissectableDataSize;
    std::unique_ptr<typename CARD::ub_CrateHeader> _crateHeader;
    bool _isValid;
    bool _isFullyDissected;
    bool _createHeaderFromData;
};

template <typename CARD>
std::vector<CARD> const&  ub_MarkedRawCrateData<CARD>::getCards() throw(datatypes_exception)
{
    if(!_isFullyDissected)
        dissectCards();

    return _markedRawCardsData;
}

template <typename CARD>
void ub_MarkedRawCrateData<CARD>::dissectCards() throw(datatypes_exception)
{
    try
    {
        dissector_type<CARD> dissector(data());
        dissector.populateCardDataVector(_markedRawCardsData);
        _isFullyDissected=true;
        _dissectableDataSize=minsize()+dissector.getTrueDataSize();
        assert(_dissectableDataSize > minsize());
        assert(_dissectableDataSize <= rawdata().size());
    }
    catch(std::exception &ex){
        throw;
    }catch(...){
        throw datatypes_exception("Caught unknown exception in ub_MarkedRawCrateData::dissectCards().");
    }
}

template <typename CARD>
bool ub_MarkedRawCrateData<CARD>::initialize() throw(datatypes_exception)
{
/*
    try
    {    if(_createHeaderFromData)
	    return crateHeader()->initialize(rawdata());
	 return true;
    } catch(std::exception &ex) {
        std::cerr << "Exception:" << ex.what() << std::endl;
        return false;
    } catch(...) {
        std::cerr << "Unknown exception.";
        return false;
    }
*/
return true;
}



template <typename CARD>
bool ub_MarkedRawCrateData<CARD>::canFullyDissect() noexcept
{
    try
    {
        dissectCards();
        crateHeader();
    } catch(std::exception &ex) {
        std::cerr << "Exception:" << ex.what() << std::endl;
        return false;
    }catch(...){
        std::cerr << "Caught unknown exception in ub_MarkedRawCrateData::dissectCards().";
        return false;
    }
    return true;
}

template <typename CARD>
std::unique_ptr<typename CARD::ub_CrateHeader>& ub_MarkedRawCrateData<CARD>::crateHeader() throw(datatypes_exception)
{
    if(_crateHeader)
        return _crateHeader;

    assert(!getCards().empty());

    std::unique_ptr<typename CARD::ub_CrateHeader> crateHeader {
        new typename CARD::ub_CrateHeader(getCards().begin()->header())
    };

    initialize();
    
    _crateHeader.swap(crateHeader);

    return _crateHeader;
}

template <typename CARD>
bool ub_MarkedRawCrateData<CARD>::compare(ub_MarkedRawCrateData<CARD> const& marked_data,bool do_rethrow) const throw(datatypes_exception)
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
        std::cerr << "Unknown exception in ub_MarkedRawCrateData<CARD>::compare()";
        if(do_rethrow)
            throw datatypes_exception("Unknown exception in ub_MarkedRawCrateData<CARD>::compare()");
        else
            return false;
    }
    return true;
}

template <typename CARD>
std::string ub_MarkedRawCrateData<CARD>::debugInfo()const noexcept
{
    std::size_t idx{0};
    std::ostringstream os;
    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << header().debugInfo();
    os << trailer().debugInfo();

    os << " *Found " << std::dec << getCards().size() << " cards." << std::endl;  
    
    for(auto card : getCards())
	os << "Card " << ++idx << std::endl << card.debugInfo();

    //os <<  ub_MarkedRawDataBlock::debugInfo();
    return os.str();
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCRATEDATA_H

