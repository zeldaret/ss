#ifndef NW4R_SND_PLAYER_HEAP_H
#define NW4R_SND_PLAYER_HEAP_H
#include "nw4r/snd/snd_SoundMemoryAllocatable.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"


namespace nw4r {
namespace snd {

// Forward declarations
class SoundPlayer;

namespace detail {

// Forward declarations
class BasicSound;

class PlayerHeap : public SoundMemoryAllocatable {
public:
    PlayerHeap();
    virtual ~PlayerHeap(); // at 0x8

    void AttachSound(BasicSound *pSound);
    void DetachSound(BasicSound *pSound);

    void SetSoundPlayer(SoundPlayer *pPlayer) {
        mPlayer = pPlayer;
    }

    void Clear();
    bool Create(void *buf, u32 size);
    void *Alloc(u32 size);
    u32 GetFreeSize() const;


private:
    BasicSound *mSound;   // at 0x4
    SoundPlayer *mPlayer; // at 0x8
    void *mStartAddress; // at 0xC
    void *mEndAddress; // at 0x10
    void *mAllocAddress; // at 0x14
public:
    NW4R_UT_LIST_NODE_DECL(); // at 0x18
};

NW4R_UT_LIST_TYPEDEF_DECL(PlayerHeap);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
