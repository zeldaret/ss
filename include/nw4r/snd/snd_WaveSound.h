#ifndef NW4R_SND_WAVESOUND_H
#define NW4R_SND_WAVESOUND_H
#include "types_nw4r.h"
#include "snd_BasicSound.h"
#include "snd_WsdPlayer.h"
#include "ut_RuntimeTypeInfo.h"

namespace nw4r
{
    namespace snd
    {
        namespace detail
        {
            struct WaveSound : BasicSound
            {
                WsdPlayer mPlayer; // at 0xD8
                WaveSoundHandle *mTempSpecialHandle; // at 0x1B0
                SoundInstanceManager<WaveSound> *mManager; // at 0x1B4
                bool mIsPrepared; // at 0x1B8

                WaveSound(SoundInstanceManager<WaveSound> *);
                bool Prepare(const void *, long, WsdPlayer::StartOffsetType, s32, int, const WsdPlayer::WsdCallback *, u32);
                void SetChannelPriority(int);
                void SetReleasePriorityFix(bool);

                static ut::detail::RuntimeTypeInfo typeInfo;
                
                virtual const ut::detail::RuntimeTypeInfo * GetRuntimeTypeInfo() const; // at 0x8
                virtual ~WaveSound(); // at 0xc
                virtual UNKTYPE Shutdown(); // at 0x28
                virtual bool IsPrepared() const; // at 0x2c
                virtual void SetPlayerPriority(int); // at 0x4c
                virtual bool IsAttachedTempSpecialHandle(); // at 0x5c
                virtual UNKTYPE DetachTempSpecialHandle(); // at 0x60
                virtual BasicPlayer * GetBasicPlayer(); // at 0x68
                virtual const BasicPlayer * GetBasicPlayer() const; // at 0x6c
            };
        }
    }
}

#endif