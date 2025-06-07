#ifndef EGG_AUDIO_HEAP_MANAGER_H
#define EGG_AUDIO_HEAP_MANAGER_H

#include "egg/core/eggAllocator.h"
#include "nw4r/snd/snd_SoundHeap.h"

namespace EGG {

class SoundMessageQueue {
public:
    SoundMessageQueue() {
        OSInitMessageQueue(&mQueue, mBuffer, 4);
    }

    OSMessageQueue mQueue;
    OSMessage mBuffer[4];
};

class SoundHeapMgr {
public:
    ~SoundHeapMgr() {
        destroySoundHeap();
    }
    int saveState() {
        return mSoundHeap.SaveState();
    }
    virtual bool loadState(s32 id) {
        s32 level = mSoundHeap.GetCurrentLevel();
        if (id > 0 && level >= id) {
            mSoundHeap.LoadState(id);
            return true;
        } else {
            return false;
        }
    }
    virtual int getCurrentLevel() {
        return mSoundHeap.GetCurrentLevel();
    }
    virtual void stateProc() {
        OSMessage msg;
        if (OSReceiveMessage(&mQueue1.mQueue, &msg, 0)) {
            bool ok = loadState((s32)msg);
            OSSendMessage(&mQueue3.mQueue, (OSMessage)ok, 0);
        }

        if (OSReceiveMessage(&mQueue2.mQueue, &msg, 0)) {
            int ok = mSoundHeap.SaveState();
            OSSendMessage(&mQueue3.mQueue, (OSMessage)ok, 0);
        }

        if (OSReceiveMessage(&mQueue, &msg, 0)) {
            int level = getCurrentLevel();
            OSSendMessage(&mQueue3.mQueue, (OSMessage)level, 0);
        }
    }

    void createSoundHeap(Allocator *allocator, u32 size);
    void destroySoundHeap();

    nw4r::snd::SoundHeap *getSoundHeap() {
        return &mSoundHeap;
    }

private:
    nw4r::snd::SoundHeap mSoundHeap;
    // I guess we're not initializing this queue?
    OSMessageQueue mQueue;
    OSMessage mBuffer[4];
    SoundMessageQueue mQueue1;
    SoundMessageQueue mQueue2;
    SoundMessageQueue mQueue3;
};

} // namespace EGG

#endif
