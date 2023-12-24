#ifndef NW4R_SND_STRMSOUND_H
#define NW4R_SND_STRMSOUND_H
#include "types_nw4r.h"
#include "snd_BasicSound.h"
#include "snd_StrmPlayer.h"
#include "ut_RuntimeTypeInfo.h"

namespace nw4r
{
    namespace snd
    {
        namespace detail
        {
            struct StrmSound : BasicSound
            {
                StrmPlayer mPlayer; // at 0xD8
                StrmSoundHandle *mTempSpecialHandle; // at 0x920
                SoundInstanceManager<StrmSound> *mManager; // at 0x924

                StrmSound(SoundInstanceManager<StrmSound> *);
                bool Prepare(StrmBufferPool *, StrmPlayer::StartOffsetType, s32, int, ut::FileStream *);

                static ut::detail::RuntimeTypeInfo typeInfo;
                
                virtual const ut::detail::RuntimeTypeInfo * GetRuntimeTypeInfo() const; // at 0x8
                virtual ~StrmSound(); // at 0xc
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