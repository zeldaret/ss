#ifndef NW4R_SND_WSDPLAYER_H
#define NW4R_SND_WSDPLAYER_H
#include "types_nw4r.h"
#include "snd_BasicPlayer.h"
#include "snd_DisposeCallback.h"
#include "snd_SoundThread.h"
#include "snd_Lfo.h"

namespace nw4r
{
    namespace snd
    {
        namespace detail
        {
            struct WsdPlayer : BasicPlayer, DisposeCallback, SoundThread::PlayerCallback
            {
                enum StartOffsetType
                {

                };

                virtual ~WsdPlayer() {} // at 0x8
                virtual bool Start(); // at 0xc
                virtual void Stop(); // at 0x10
                virtual void Pause(bool); // at 0x14
                virtual bool IsActive() const; // at 0x18
                virtual bool IsStarted() const; // at 0x1c
                virtual bool IsPause() const; // at 0x20

                virtual void OnUpdateFrameSoundThread(); // at 0xc
                virtual void OnUpdateVoiceSoundThread(); // at 0x10
                virtual void OnShutdownSoundThread(); // at 0x14

                virtual void InvalidateData(const void *, const void *); // at 0xc
                virtual void InvalidateWaveData(const void *, const void *); // at 0x10

                struct WsdCallback
                {
                    virtual ~WsdCallback();
                };

                WsdPlayer();
                bool Prepare(const void *, int, WsdPlayer::StartOffsetType, int, int, const WsdPlayer::WsdCallback *, u32);
                void SetChannelPriority(int);
                void SetReleasePriorityFix(bool);

                char UNK_0xE4[0x4C];
            };
        }
    }
}

#endif