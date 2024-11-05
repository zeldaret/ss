#include "nw4r/snd/snd_PlayerHeap.h"

#include "common.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/ut.h"

namespace nw4r {
namespace snd {
namespace detail {

PlayerHeap::PlayerHeap() : mSound(NULL), mPlayer(NULL), mStartAddress(NULL), mEndAddress(NULL), mAllocAddress(NULL) {}
PlayerHeap::~PlayerHeap() {
    Clear();
    mAllocAddress = NULL;
}

bool PlayerHeap::Create(void *buf, u32 size) {
    void *aligned = ROUND_UP_PTR(buf, 0x20);
    if (aligned > ut::AddOffsetToPtr(buf, size)) {
        return false;
    }

    mStartAddress = aligned;
    mEndAddress = ut::AddOffsetToPtr(buf, size);
    mAllocAddress = aligned;

    return true;
}
void *PlayerHeap::Alloc(u32 size) {
    if (ut::AddOffsetToPtr(mAllocAddress, size) > mEndAddress) {
        return NULL;
    }
    void *oldAllocAddress = mAllocAddress;
    mAllocAddress = ROUND_UP_PTR(ut::AddOffsetToPtr(mAllocAddress, size), 0x20);
    return oldAllocAddress;
}

void PlayerHeap::Clear() {
    SoundThread::AutoLock lock;
    DisposeCallbackManager::GetInstance().Dispose(mStartAddress, (u8 *)mAllocAddress - (u8 *)mStartAddress, NULL);
    DisposeCallbackManager::GetInstance().DisposeWave(mStartAddress, (u8 *)mAllocAddress - (u8 *)mStartAddress, NULL);
    mAllocAddress = mStartAddress;
}

u32 PlayerHeap::GetFreeSize() const {
    return (u8 *)mEndAddress - (u8 *)mAllocAddress;
}

void PlayerHeap::AttachSound(BasicSound *pSound) {
    mSound = pSound;
}

void PlayerHeap::DetachSound(BasicSound *pSound) {
    mSound = NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
