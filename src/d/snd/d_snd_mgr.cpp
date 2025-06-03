#include "d/snd/d_snd_mgr.h"

#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_util.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "egg/audio/eggAudioUtility.h"

extern "C" void initEnemySoundMgr();
extern "C" void initFanfareSoundMgr();
extern "C" void initSomeUnusedSoundMgr();
extern "C" void initSoundEffectSoundMgr();
extern "C" void fn_8036A430();
extern "C" void initEnemyBgmRelatedMgr();
extern "C" void fn_80393530();
extern "C" void fn_80394830();
extern "C" void fn_8037F940();
extern "C" void fn_80399600();
extern "C" void fn_80399C20();

dSndMgr_c *dSndMgr_c::sInstance;

dSndMgr_c::dSndMgr_c(): field_0x6CC(0) {
    sInstance = this;
    SndMgrDisposer<dSndPlayerMgr_c>::create();
    SndMgrDisposer<dSndControlPlayerMgr_c>::create();
    initEnemySoundMgr();
    SndMgrDisposer<dSnd3DManager_c>::create();
    initFanfareSoundMgr();
    initSomeUnusedSoundMgr();
    initSoundEffectSoundMgr();
    fn_8036A430();
    initEnemyBgmRelatedMgr();
    fn_80393530();
    fn_80394830();
    fn_8037F940();
    fn_80399600();
    fn_80399C20();

    initHbm(9);
}

void dSndMgr_c::initialize(EGG::Heap *heap, u32 size) {}

void dSndMgr_c::initHbm(u32 frame) {
    EGG::AudioUtility::HBM::init(this, restoreEffectsCallback, frame);
}

void dSndMgr_c::calc() {
    if (field_0x6CC) {
        EGG::SimpleAudioMgr::calc();
        EGG::AudioRmtSpeakerMgr::calc();
        dSndPlayerMgr_c::GetInstance()->calc();
    }
}

void dSndMgr_c::restoreEffectsCallback() {}
