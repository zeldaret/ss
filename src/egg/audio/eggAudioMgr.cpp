#include <egg/audio/eggAudioMgr.h>
#include <nw4r/snd/snd_SoundSystem.h>
#include <rvl/AI.h>
#include <rvl/AX.h>

namespace EGG {

IAudioMgr::Arg::Arg() {
    heap = nullptr;
    sndThreadPriority = 4;
    dvdThreadPriority = 3;
    soundFileName = nullptr;
    sndThreadStackSize = 0;
    dvdThreadStackSize = 0;
    blocks = 1;
}

SimpleAudioMgr::SimpleAudioMgrArg::SimpleAudioMgrArg() {
    field_0x1C = 0x8CA000;
    loadStringLabels = true;
}

SimpleAudioMgr::SimpleAudioMgr() : ArcPlayer(&mArchivePlayer, getSoundHeap()) {
    init();
}

SimpleAudioMgr::~SimpleAudioMgr() {}

void SimpleAudioMgr::initialize(EGG::IAudioMgr::Arg *arg) {
    AIInit(nullptr);
    AXInit();

    if (arg->field_0x1C != 0 && arg->heap != nullptr) {
        Allocator alloc(arg->heap, 0x20);
        SoundHeapMgr::createSoundHeap(&alloc, arg->field_0x1C);
    }

    if (arg->sndThreadStackSize == 0 && arg->dvdThreadStackSize == 0) {
        nw4r::snd::SoundSystem::InitSoundSystem(arg->sndThreadPriority, arg->dvdThreadPriority);
    } else {
        if (arg->sndThreadStackSize == 0) {
            arg->sndThreadStackSize = 0x4000;
        }
        if (arg->dvdThreadStackSize == 0) {
            arg->dvdThreadStackSize = 0x4000;
        }

        nw4r::snd::SoundSystem::SoundSystemParam param;
        param.soundThreadPriority = arg->sndThreadPriority;
        param.soundThreadStackSize = arg->sndThreadStackSize;
        param.dvdThreadPriority = arg->dvdThreadPriority;
        param.dvdThreadStackSize = arg->dvdThreadStackSize;
        u32 size = nw4r::snd::SoundSystem::GetRequiredMemSize(param);
        void *ptr = SoundHeapMgr::getSoundHeap()->Alloc(size, nullptr, (void*)'SYST');
        nw4r::snd::SoundSystem::InitSoundSystem(param, ptr, size);
    }
    ArcPlayer::setSteamBlocks(arg->blocks);
    setLoadStringLabels(arg->loadStringLabels);
    IAudioMgr::field_0x04 = true;
}

void SimpleAudioMgr::calc() {
    AudioSystem::calc();
    if (AudioSystem::isField0x04Eq2()) {
        ArcPlayer::stopAllSound();
    }
    ArcPlayer::calc();
}

UNKTYPE *SimpleAudioMgr::openDvdArchive(const char *name, nw4r::snd::SoundHeap *heap) {
    return ArcPlayer::openDvdArchive(name, heap);
}
UNKTYPE *SimpleAudioMgr::openNandArchive(const char *name, nw4r::snd::SoundHeap *heap) {
    return ArcPlayer::openNandArchive(name, heap);
}
UNKTYPE *SimpleAudioMgr::setupMemoryArchive(const void *name, nw4r::snd::SoundHeap *heap) {
    return ArcPlayer::setupMemoryArchive(name, heap);
}

void SimpleAudioMgr::closeArchive() {
    ArcPlayer::closeArchive();
}
bool SimpleAudioMgr::loadGroup(unsigned int arg, nw4r::snd::SoundHeap *heap, u32 arg2) {
    return ArcPlayer::loadGroup(arg, heap, arg2);
}
bool SimpleAudioMgr::loadGroup(int arg, nw4r::snd::SoundHeap *heap, u32 arg2) {
    return ArcPlayer::loadGroup(arg, heap, arg2);
}
bool SimpleAudioMgr::loadGroup(u32 arg, nw4r::snd::SoundHeap *heap, u32 arg2) {
    return ArcPlayer::loadGroup(arg, heap, arg2);
}
bool SimpleAudioMgr::loadGroup(const char *arg, nw4r::snd::SoundHeap *heap, u32 arg2) {
    return ArcPlayer::loadGroup(arg, heap, arg2);
}

} // namespace EGG
