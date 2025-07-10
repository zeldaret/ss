#ifndef EGG_AUDIO_MANAGER_H
#define EGG_AUDIO_MANAGER_H

#include "egg/audio/eggAudioArcPlayerMgr.h"
#include "egg/audio/eggAudioHeapMgr.h"
#include "egg/audio/eggAudioSystem.h"
#include "egg/core/eggHeap.h"
#include "egg/egg_types.h"

namespace EGG {

class SoundArchivePlayerEGG : public nw4r::snd::SoundArchivePlayer {
public:
    virtual StartResult
    detail_SetupSound(nw4r::snd::SoundHandle *pHandle, u32 id, bool hold, const StartInfo *pStartInfo) override {
        if (AudioSystem::sInstanse != nullptr &&
            (AudioSystem::sInstanse->field0x08NotZero() || AudioSystem::sInstanse->field0x04NotZero())) {
            return START_ERR_USER;
        }
        return nw4r::snd::SoundArchivePlayer::detail_SetupSound(pHandle, id, hold, pStartInfo);
    } // at 0x2C
};

class IAudioMgr {
public:
    struct Arg {
        Arg();
        /* 0x00 */ EGG::Heap *heap;
        /* 0x04 */ const char *soundFileName;
        /* 0x08 */ int sndThreadPriority;
        /* 0x0C */ int dvdThreadPriority;
        /* 0x10 */ int sndThreadStackSize;
        /* 0x14 */ int dvdThreadStackSize;
        /* 0x18 */ u32 blocks;
        /* 0x1c */ u32 field_0x1C;
        /* 0x20 */ bool loadStringLabels;
    };

    void init() {
        field_0x04 = false;
    }

    virtual void initialize(Arg *) {}
    virtual UNKTYPE calc() = 0;

    bool field_0x04;
};

class SimpleAudioMgr : public IAudioMgr, public SoundHeapMgr, public ArcPlayer, public AudioSystem {
public:
    struct SimpleAudioMgrArg : public IAudioMgr::Arg {
        SimpleAudioMgrArg();
    };

    SimpleAudioMgr();
    virtual ~SimpleAudioMgr();

    virtual void initialize(EGG::IAudioMgr::Arg *) override;
    virtual void calc() override;

    virtual UNKTYPE *openDvdArchive(const char *, nw4r::snd::SoundHeap *) override;     // at 0x10
    virtual UNKTYPE *openNandArchive(const char *, nw4r::snd::SoundHeap *) override;    // at 0x14
    virtual UNKTYPE *setupMemoryArchive(const void *, nw4r::snd::SoundHeap *) override; // at 0x18

    virtual void closeArchive() override;                                       // at 0x20
    virtual bool loadGroup(unsigned int, nw4r::snd::SoundHeap *, u32) override; // at 0x24
    virtual bool loadGroup(int, nw4r::snd::SoundHeap *, u32) override;          // at 0x28
    virtual bool loadGroup(u32, nw4r::snd::SoundHeap *, u32) override;          // at 0x2C
    virtual bool loadGroup(const char *, nw4r::snd::SoundHeap *, u32) override; // at 0x30

private:
    SoundArchivePlayerEGG mArchivePlayer;
};

} // namespace EGG

#endif
