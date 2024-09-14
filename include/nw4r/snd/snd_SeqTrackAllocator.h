#ifndef NW4R_SND_SEQ_TRACK_ALLOCATOR_H
#define NW4R_SND_SEQ_TRACK_ALLOCATOR_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class SeqPlayer;
class SeqTrack;

class SeqTrackAllocator {
public:
    virtual ~SeqTrackAllocator() {} // at 0x8

    virtual SeqTrack* AllocTrack(SeqPlayer* pPlayer) = 0; // at 0xC
    virtual void FreeTrack(SeqTrack* pTrack) = 0;         // at 0x10
    virtual int GetAllocatableTrackCount() const = 0;     // at 0x14
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
