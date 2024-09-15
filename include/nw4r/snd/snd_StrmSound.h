#ifndef NW4R_SND_STRM_SOUND_H
#define NW4R_SND_STRM_SOUND_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_StrmPlayer.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Forward declarations
class StrmSoundHandle;

namespace detail {

// Forward declarations
template <typename T> class SoundInstanceManager;

class StrmSound : public BasicSound {
    friend class StrmSoundHandle;

public:
    NW4R_UT_RTTI_DECL(StrmSound);

public:
    explicit StrmSound(SoundInstanceManager<StrmSound>* pManager);

    virtual void Shutdown(); // at 0x28
    virtual bool IsPrepared() const {
        return mStrmPlayer.IsPrepared();
    } // at 0x2C

    virtual void SetPlayerPriority(int priority); // at 0x4C
    virtual bool IsAttachedTempSpecialHandle();   // at 0x5C
    virtual void DetachTempSpecialHandle();       // at 0x60

    virtual BasicPlayer& GetBasicPlayer() {
        return mStrmPlayer;
    } // at 0x68
    virtual const BasicPlayer& GetBasicPlayer() const {
        return mStrmPlayer;
    } // at 0x6C

    bool Prepare(StrmBufferPool* pPool, StrmPlayer::StartOffsetType offsetType,
                 s32 offset, int voices, ut::FileStream* pStream);

    void* GetFileStreamBuffer() {
        return mFileStreamBuffer;
    }
    s32 GetFileStreamBufferSize() {
        return sizeof(mFileStreamBuffer);
    }

private:
    static const int FILE_STREAM_BUFFER_SIZE = 512;

private:
    StrmPlayer mStrmPlayer;                    // at 0xD8
    StrmSoundHandle* mTempSpecialHandle;       // at 0x920
    SoundInstanceManager<StrmSound>* mManager; // at 0x924
    char UNK_0x928[0x93C - 0x928];
    char mFileStreamBuffer[FILE_STREAM_BUFFER_SIZE]; // at 0x93C
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
