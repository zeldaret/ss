#include "d/snd/d_snd_mgr.h"

#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_area_sound_effect_mgr.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_rng_mgr.h"
#include "d/snd/d_snd_se_sound_pool.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "egg/audio/eggAudioUtility.h"

extern "C" void initSomeUnusedSoundMgr();
extern "C" void fn_8037F940();
extern "C" void fn_80399600();

dSndMgr_c *dSndMgr_c::sInstance;

dSndMgr_c::dSndMgr_c() : field_0x6CC(0) {
    sInstance = this;
    dSndPlayerMgr_c::create();
    dSndControlPlayerMgr_c::create();
    dSndStateMgr_c::create();
    dSnd3DManager_c::create();
    dSndBgmMgr_c::create();
    initSomeUnusedSoundMgr();
    dSndSmallEffectMgr_c::create();
    dSndAreaSoundEffectMgr_c::create();
    dSndSourceMgr_c::create();
    dSndDistantSoundActorPool_c::create();
    dSndSeSoundPool_c::create();
    fn_8037F940();
    fn_80399600();
    dSndRngMgr_c::create();

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

void dSndMgr_c::restoreEffectsCallback() {
    dSndStateMgr_c::GetInstance()->restoreEffects();
    dSndBgmMgr_c::GetInstance()->restoreEffects();
}
