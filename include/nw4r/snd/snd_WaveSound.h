#ifndef NW4R_SND_WAVE_SOUND_H
#define NW4R_SND_WAVE_SOUND_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_WsdPlayer.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Forward declarations
class WaveSoundHandle;

namespace detail {

// Forward declarations
template <typename T> class SoundInstanceManager;

class WaveSound : public BasicSound {
    friend class WaveSoundHandle;

public:
    NW4R_UT_RTTI_DECL(WaveSound);

public:
    explicit WaveSound(SoundInstanceManager<WaveSound>* pManager);

    virtual void Shutdown(); // at 0x28
    virtual bool IsPrepared() const {
        return mPreparedFlag;
    } // at 0x2C

    virtual void SetPlayerPriority(int priority); // at 0x4C
    virtual bool IsAttachedTempSpecialHandle();   // at 0x5C
    virtual void DetachTempSpecialHandle();       // at 0x60

    virtual BasicPlayer& GetBasicPlayer() {
        return mWsdPlayer;
    } // at 0x68
    virtual const BasicPlayer& GetBasicPlayer() const {
        return mWsdPlayer;
    } // at 0x6C

    bool Prepare(const void* pWsdData, s32 wsdOffset,
                 WsdPlayer::StartOffsetType startType, s32 startOffset,
                 int voices, const WsdPlayer::WsdCallback* pCallback,
                 u32 callbackArg);

    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

private:
    WsdPlayer mWsdPlayer;                      // at 0xD8
    WaveSoundHandle* mTempSpecialHandle;       // at 0x1B0
    SoundInstanceManager<WaveSound>* mManager; // at 0x1B4
    bool mPreparedFlag;                        // at 0x1B8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
