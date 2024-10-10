#ifndef EGG_AUDIO_HEAP_MANAGER_H
#define EGG_AUDIO_HEAP_MANAGER_H

#include <egg/core/eggAllocator.h>
#include <nw4r/snd/snd_SoundHeap.h>

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
    virtual bool loadState(s32 id) {}
    virtual int getCurrentLevel() {}
    virtual void stateProc() {}

    void createSoundHeap(Allocator *allocator, u32 size);
    void destroySoundHeap();

    nw4r::snd::SoundHeap *getSoundHeap() {
        return &mSoundHeap;
    }

private:
    nw4r::snd::SoundHeap mSoundHeap;
    u8 unk[0x68 - 0x38];
    SoundMessageQueue mQueue1;
    SoundMessageQueue mQueue2;
    SoundMessageQueue mQueue3;
};

} // namespace EGG

#endif
