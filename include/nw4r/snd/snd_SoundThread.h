#ifndef NW4R_SND_SOUND_THREAD_H
#define NW4R_SND_SOUND_THREAD_H
#include "common.h"
#include "snd_AxManager.h"
#include "ut_LinkList.h"
#include <rvl/OS/OSMessage.h>
#include <rvl/OS/OSMutex.h>
#include <rvl/OS/OSThread.h>


namespace nw4r {
namespace snd {
namespace detail {
class SoundThread {
public:
    struct SoundThreadCallback {
        ut::LinkListNode mNode; // at 0x0

        inline virtual ~SoundThreadCallback() {} // at 0x8
        inline virtual void VF_0xC() {}          // at 0xC
        inline virtual void VF_0x10() {}         // at 0x10
    };

    struct PlayerCallback {
        ut::LinkListNode mNode; // at 0x0

        inline virtual ~PlayerCallback() {}               // at 0x8
        inline virtual void OnUpdateFrameSoundThread() {} // at 0xc
        inline virtual void OnUpdateVoiceSoundThread() {} // at 0x10
        inline virtual void OnShutdownSoundThread() {}    // at 0x14
    };

    struct AutoLock {
        inline AutoLock() {
            GetInstance()->Lock();
        }
        inline ~AutoLock() {
            GetInstance()->Unlock();
        }
    };

public:
    static SoundThread *GetInstance();

    SoundThread();
    ~SoundThread() {}

    void Lock() {
        OSLockMutex(&mMutex);
    }
    void Unlock() {
        OSUnlockMutex(&mMutex);
    }

    bool Create(s32, void *, u32);
    void Shutdown();
    static void AxCallbackFunc();
    void AxCallbackProc();
    static void *SoundThreadFunc(void *);
    void RegisterPlayerCallback(PlayerCallback *);
    void UnregisterPlayerCallback(PlayerCallback *);
    void SoundThreadProc();

private:
    OSThread mThread;           // at 0x0
    OSThreadQueue mThreadQueue; // at 0x318
    OSMessageQueue mMesgQueue;  // at 0x320
    OSMessage mMesgBuffer;      // at 0x340
    char UNK_0x344[0x350 - 0x344];
    void *mStackEnd;                                    // at 0x350
    OSMutex mMutex;                                     // at 0x354
    AxManager::CallbackListNode mAxNode;                // at 0x36C
    ut::LinkList<SoundThreadCallback, 0> mThreadCbList; // at 0x378
    ut::LinkList<PlayerCallback, 0> mPlayerCbList;      // at 0x384
    s32 mLastUpdateTime;                                // at 0x390
    bool mIsCreated;                                    // at 0x394
};
} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
