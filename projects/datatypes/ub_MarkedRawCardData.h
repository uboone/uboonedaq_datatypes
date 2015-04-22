#ifndef _UBOONE_TYPES_MARKEDRAWCARDDATA_H
#define _UBOONE_TYPES_MARKEDRAWCARDDATA_H 1

#include "uboone_data_utils.h"
#include "uboone_data_internals.h"
#include "ub_MarkedRawChannelData.h"
#include "ub_ChannelDataCreatorHelperClass.h"

namespace gov {
namespace fnal {
namespace uboone {
namespace datatypes {

template <typename CHANN, typename HEADER,typename TRAILER>
class ub_MarkedRawCardData : public ub_MarkedRawDataBlock<HEADER,TRAILER> {
public:
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::header_type card_header_type;
    typedef typename ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer_type card_trailer_type;
    typedef CHANN card_channel_type;

    static constexpr size_t size_of_data_overhead() {
        return size_of<card_header_type>() + size_of<card_trailer_type>();
    }

    template <typename MRCD> using dissector_type = ub_ChannelDataCreatorHelperClass<MRCD>;

    explicit ub_MarkedRawCardData(ub_RawData const rawdata):
        ub_MarkedRawDataBlock<HEADER,TRAILER>(rawdata),
        _markedRawChannelsData {},_isValid {isValid()},_isFullyDissected {canFullyDissect()} {}
    //_markedRawChannelsData{},_isValid{isValid()},_isFullyDissected{false}{}

    uint32_t const& getCardIDAndModuleWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().id_and_module;
    }
    uint32_t const& getCardWordCountWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().word_count;
    }
    uint32_t const& getCardEventWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().event_number;
    }
    uint32_t const& getCardFrameWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().frame_number;
    }
    uint32_t const& getCardChecksumWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().checksum;
    }
    uint32_t const& getCardTrigFrameAndSampleWord() const noexcept{
        return ub_MarkedRawDataBlock<HEADER,TRAILER>::header().trig_frame_and_sample;
    }

    bool compare(ub_MarkedRawCardData const&,bool do_rethrow=false) const throw(datatypes_exception);

    std::vector<CHANN>  const&  getChannels() throw(datatypes_exception);
    std::vector<CHANN>  const&  getChannels()const noexcept{
        return _markedRawChannelsData;
    };

    ub_MarkedRawCardData() = delete;
    ub_MarkedRawCardData& operator=(ub_MarkedRawCardData const &) = delete;

    void decompressChannels() throw(datatypes_exception);
    void dissectChannels() throw(datatypes_exception);
    std::string debugInfo()const noexcept;

private:
    bool isValid() noexcept;
    bool canFullyDissect() noexcept;
private:
    std::vector<CHANN>  _markedRawChannelsData;
    bool _isValid;
    bool _isFullyDissected;
};

template <typename CHANN, typename HEADER,typename TRAILER>
std::vector<CHANN>  const&  ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::getChannels() throw(datatypes_exception)
{
    if(!_isFullyDissected)
        dissectChannels();

    return _markedRawChannelsData;
}

template <typename CHANN, typename HEADER,typename TRAILER>
void ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::dissectChannels() throw(datatypes_exception)
{
    try
    {
        dissector_type<CHANN> dissector(ub_MarkedRawDataBlock<HEADER,TRAILER>::data());
        dissector.populateChannelDataVector(_markedRawChannelsData);
        _isFullyDissected=true;
    }
    catch(datatypes_exception &ex){
        
        throw;
    }catch(std::exception &e){
         throw datatypes_exception(std::string("Caught std::exception in ub_MarkedRawCardData::dissectChannels(). Message:").append(e.what()));
    }catch(...){
        throw datatypes_exception("Caught unknown exception in ub_MarkedRawCardData::dissectChannels().");
    }
}

template <typename CHANN, typename HEADER,typename TRAILER>
bool ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::canFullyDissect() noexcept
{
    try
    {
        dissectChannels();
    }
    catch(std::exception &ex){
        std::cerr << "Exception:" << ex.what() << std::endl;
//        std::cerr << debugInfo() << std::endl;
        return false;
    }catch(...){
        std::cerr << "Caught unknown exception ub_MarkedRawCardData::canFullyDissect()" << std::endl;
//        std::cerr << debugInfo() << std::endl;
        return false;
    }
    
    return true;
}

template <typename CHANN, typename HEADER,typename TRAILER>
std::string ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::debugInfo()const noexcept
{
    std::ostringstream os;

    os << "Object " << demangle(typeid(this)) << "."<< std::endl;
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::header().debugInfo();
    os << ub_MarkedRawDataBlock<HEADER,TRAILER>::trailer().debugInfo();

    os << " *Found " << std::dec << getChannels().size() << " channels." << std::endl;
    for(auto chan : getChannels())
    {
        if(chan.rawdata().size() < 0x200)
            os << chan.debugInfo();
        else
           os << debugInfoShort(chan.rawdata());
    }

    // os <<  ub_MarkedRawDataBlock::debugInfo();
    return os.str();
}
template <typename CHANN, typename HEADER,typename TRAILER>
bool ub_MarkedRawCardData<CHANN, HEADER,TRAILER>::compare(ub_MarkedRawCardData const& card_data,bool do_rethrow) const throw(datatypes_exception)
{
    UNUSED(card_data);
    UNUSED(do_rethrow);
    return true;
}

}  // end of namespace datatypes
}  // end of namespace uboone
}  // end of namespace fnal
}

#endif //_UBOONE_TYPES_MARKEDRAWCARDDATA_H
