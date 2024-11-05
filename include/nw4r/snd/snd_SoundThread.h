#ifndef NW4R_SND_SOUND_THREAD_H
#define NW4R_SND_SOUND_THREAD_H
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/OS.h"  // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

class SoundThread {
    friend class AutoLock; // Prevent locking without AutoLock

public:
    class SoundFrameCallback {
    public:
        NW4R_UT_LIST_NODE_DECL(); // at 0x0

        virtual ~SoundFrameCallback() {}    // at 0x8
        virtual void OnBeginSoundFrame() {} // at 0xC
        virtual void OnEndSoundFrame() {}   // at 0x10
    };

    NW4R_UT_LIST_TYPEDEF_DECL(SoundFrameCallback);

    class PlayerCallback {
    public:
        NW4R_UT_LIST_NODE_DECL(); // at 0x0

        virtual ~PlayerCallback() {}               // at 0x8
        virtual void OnUpdateFrameSoundThread() {} // at 0xC
        virtual void OnUpdateVoiceSoundThread() {} // at 0x10
        virtual void OnShutdownSoundThread() {}    // at 0x14
    };

    NW4R_UT_LIST_TYPEDEF_DECL(PlayerCallback);

    class AutoLock : private ut::NonCopyable {
    public:
        AutoLock() {
            SoundThread::GetInstance().Lock();
        }

        ~AutoLock() {
            SoundThread::GetInstance().Unlock();
        }
    };

public:
    static SoundThread &GetInstance();

    bool Create(s32 priority, void *pStack, u32 stackSize);
    void Shutdown();

    void RegisterPlayerCallback(PlayerCallback *pCallback);
    void UnregisterPlayerCallback(PlayerCallback *pCallback);

private:
    enum ThreadMessage {
        MSG_NONE,
        MSG_AX_CALLBACK,
        MSG_SHUTDOWN,
    };

    static const int MSG_QUEUE_CAPACITY = 4;

private:
    SoundThread();

    static void AxCallbackFunc();
    void AxCallbackProc();

    static void *SoundThreadFunc(void *pArg);
    void SoundThreadProc();
    void FrameProcess();

    void Lock() {
        OSLockMutex(&mMutex);
    }
    void Unlock() {
        OSUnlockMutex(&mMutex);
    }

private:
    OSThread mThread;                         // at 0x0
    OSThreadQueue mThreadQueue;               // at 0x318
    OSMessageQueue mMsgQueue;                 // at 0x320
    OSMessage mMsgBuffer[MSG_QUEUE_CAPACITY]; // at 0x340
    void *mStackEnd;                          // at 0x350
    mutable OSMutex mMutex;                   // at 0x354

    AxManager::CallbackListNode mAxCallbackNode;    // at 0x36C
    SoundFrameCallbackList mSoundFrameCallbackList; // at 0x378
    PlayerCallbackList mPlayerCallbackList;         // at 0x384

    u32 mProcessTick; // at 0x390
    bool mCreateFlag; // at 0x394
    bool field_0x395; // at 0x395
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
