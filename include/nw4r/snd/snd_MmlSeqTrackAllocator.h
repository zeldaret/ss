#ifndef NW4R_SND_MML_SEQ_TRACK_ALLOCATOR_H
#define NW4R_SND_MML_SEQ_TRACK_ALLOCATOR_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_InstancePool.h>
#include <nw4r/snd/snd_SeqTrackAllocator.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class MmlParser;
class SeqPlayer;
class SeqTrack;

class MmlSeqTrackAllocator : public SeqTrackAllocator {
public:
    MmlSeqTrackAllocator(MmlParser* pParser) : mParser(pParser) {}

    virtual SeqTrack* AllocTrack(SeqPlayer* pPlayer); // at 0xC
    virtual void FreeTrack(SeqTrack* pTrack);         // at 0x10

    virtual int GetAllocatableTrackCount() const {
        return mTrackPool.Count();
    } // at 0x14

    u32 Create(void* pBuffer, u32 size);
    void Destroy(void* pBuffer, u32 size);

private:
    MmlParser* mParser;                   // at 0x4
    InstancePool<MmlSeqTrack> mTrackPool; // at 0x8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
