#ifndef NW4R_SND_STRMPLAYER_H
#define NW4R_SND_STRMPLAYER_H
#include "types_nw4r.h"
#include "snd_BasicPlayer.h"
#include "snd_SoundThread.h"

namespace nw4r
{
    namespace snd
    {
        namespace detail
        {
            struct StrmPlayer : BasicPlayer, SoundThread::PlayerCallback
            {
                enum StartOffsetType
                {

                };

                virtual ~StrmPlayer(); // at 0x8
                virtual bool Start(); // at 0xc
                virtual void Stop(); // at 0x10
                virtual void Pause(bool); // at 0x14
                virtual bool IsActive() const; // at 0x18
                virtual bool IsStarted() const; // at 0x1c
                virtual bool IsPause() const; // at 0x20

                virtual void OnUpdateFrameSoundThread(); // at 0xc
                virtual void OnUpdateVoiceSoundThread(); // at 0x10
                virtual void OnShutdownSoundThread(); // at 0x14

                struct StrmDataLoadTask
                {
                    virtual ~StrmDataLoadTask();
                    virtual UNKTYPE Execute();
                    virtual UNKTYPE Cancel();
                    virtual UNKTYPE OnCancel();
                };

                struct StrmHeaderLoadTask
                {
                    virtual ~StrmHeaderLoadTask();
                    virtual UNKTYPE Execute();
                    virtual UNKTYPE Cancel();
                    virtual UNKTYPE OnCancel();
                };

                bool IsPrepared() const { return mIsPrepared; }

                StrmPlayer();
                bool Setup(StrmBufferPool *);
                bool Prepare(ut::FileStream *, int, StartOffsetType, int);
                void Shutdown();

                char UNK_D8[0x113 - 0xD8];
                bool mIsPrepared; // at 0x113
                char UNK_120[0x78C];
            };
        }
    }
}

#endif