#include "egg/audio/eggAudioMgr.h"


// Note: Pretty much this entire file is deadstripped by the linker, but ONE artifact remains:
// The single `EGG::SimpleAudioMgr::@248@calc()` weak function.
// As a result, this file has no corresponding header, and is basically unimplemented.

namespace EGG {

class ExpAudioMgr : public SimpleAudioMgr {
public:

    ExpAudioMgr();
    virtual ~ExpAudioMgr();

    // Do not override calc()

    void initialize(EGG::IAudioMgr::Arg *) override;

    virtual UNKTYPE *openDvdArchive(const char *, nw4r::snd::SoundHeap *) override;     // at 0x10
    virtual UNKTYPE *openNandArchive(const char *, nw4r::snd::SoundHeap *) override;    // at 0x14
    virtual UNKTYPE *setupMemoryArchive(const void *, nw4r::snd::SoundHeap *) override; // at 0x18

    virtual void closeArchive() override;                                       // at 0x20
    virtual bool loadGroup(unsigned int, nw4r::snd::SoundHeap *, u32) override; // at 0x24
    virtual bool loadGroup(int, nw4r::snd::SoundHeap *, u32) override;          // at 0x28
    virtual bool loadGroup(u32, nw4r::snd::SoundHeap *, u32) override;          // at 0x2C
    virtual bool loadGroup(const char *, nw4r::snd::SoundHeap *, u32) override; // at 0x30
};


ExpAudioMgr::ExpAudioMgr() {}
ExpAudioMgr::~ExpAudioMgr() {}

} // namespace EGG
