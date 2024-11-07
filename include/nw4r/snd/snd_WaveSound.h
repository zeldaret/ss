#ifndef NW4R_SND_WAVE_SOUND_H
#define NW4R_SND_WAVE_SOUND_H
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_WsdPlayer.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"


namespace nw4r {
namespace snd {

// Forward declarations
class WaveSoundHandle;

namespace detail {

// Forward declarations
template <typename T>
class SoundInstanceManager;

class WaveSound : public BasicSound {
    friend class WaveSoundHandle;

public:
    NW4R_UT_RTTI_DECL(WaveSound);

public:
    explicit WaveSound(SoundInstanceManager<WaveSound> *pManager, int priority, int ambientPriority);

    virtual void Shutdown() override; // at 0x10
    virtual bool IsPrepared() override const {
        return mPreparedFlag;
    } // at 0x14

    void SetPlayerPriority(int priority);
    virtual bool IsAttachedTempSpecialHandle() override;   // at 0x18
    virtual void DetachTempSpecialHandle();       // at 0x1C

    virtual BasicPlayer &GetBasicPlayer() {
        return mWsdPlayer;
    } // at 0x24
    virtual const BasicPlayer &GetBasicPlayer() const {
        return mWsdPlayer;
    } // at 0x28

    virtual void OnUpdatePlayerPriority() override; // at 0x2C

    bool Prepare(
        const void *pWsdData, s32 wsdOffset, WsdPlayer::StartOffsetType startType, s32 startOffset,
        const WsdPlayer::WsdCallback *pCallback, u32 callbackArg
    );

    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

private:
    WsdPlayer mWsdPlayer;                      // at 0xD8
    WaveSoundHandle *mTempSpecialHandle;       // at 0x1B0
    SoundInstanceManager<WaveSound> *mManager; // at 0x1B4
    bool mPreparedFlag;                        // at 0x1B8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
