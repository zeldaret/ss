#include <d/snd/d_snd_mgr.h>
#include <egg/audio/eggAudioRmtSpeakerMgr.h>
#include <egg/audio/eggAudioUtility.h>

dSndMgr_c::dSndMgr_c() {
    sInstance = this;

    initHbm(9);
}

void dSndMgr_c::initialize(EGG::Heap *heap, u32 size) {}

void dSndMgr_c::initHbm(u32 frame) {
    EGG::AudioUtility::HBM::init(this, restoreEffectsCallback, frame);
}

void dSndMgr_c::calc() {
    EGG::SimpleAudioMgr::calc();
    EGG::AudioRmtSpeakerMgr::calc();
}

void dSndMgr_c::restoreEffectsCallback() {}
