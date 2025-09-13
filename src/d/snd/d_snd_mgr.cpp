#include "d/snd/d_snd_mgr.h"

#include "common.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_area_sound_effect_mgr.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_bgm_seq_data_mgr.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_fi_vocal_mgr.h"
#include "d/snd/d_snd_harp_song_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_rng_mgr.h"
#include "d/snd/d_snd_se_sound_pool.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "egg/audio/eggAudioUtility.h"
#include "nw4r/snd/snd_SoundSystem.h"

dSndMgr_c *dSndMgr_c::sInstance;

dSndMgr_c::dSndMgr_c() : mIsInitialized(false) {
    sInstance = this;
    dSndPlayerMgr_c::create();
    dSndControlPlayerMgr_c::create();
    dSndStateMgr_c::create();
    dSnd3DManager_c::create();
    dSndBgmMgr_c::create();
    dSndBgmSeqDataMgr_c::create();
    dSndSmallEffectMgr_c::create();
    dSndAreaSoundEffectMgr_c::create();
    dSndSourceMgr_c::create();
    dSndDistantSoundActorPool_c::create();
    dSndSeSoundPool_c::create();
    dSndHarpSongMgr_c::create();
    dSndFiVocalMgr_c::create();
    dSndRngMgr_c::create();

    initHbm(9);
}

void dSndMgr_c::setup(EGG::Heap *heap, u32 size) {
    if (mIsInitialized) {
        return;
    }

    SimpleAudioMgrArg arg;
    arg.heap = heap;
    arg.soundFileName = dSndPlayerMgr_c::GetInstance()->getSoundArchivePath();
    // TODO - how is this calculated?
    arg.field_0x1C = size + 0x1400;
    arg.blocks = 2;
    initialize(&arg);
    mOutputMode = nw4r::snd::SoundSystem::GetOutputMode();
    dSndPlayerMgr_c::GetInstance()->setup();
    dSndStateMgr_c::GetInstance()->setup(heap);
    dSndBgmMgr_c::GetInstance()->setup(heap);
    dSndBgmSeqDataMgr_c::GetInstance()->setup();
    dSndDistantSoundActorPool_c::GetInstance()->setup();
    dSndHarpSongMgr_c::GetInstance()->setup();
    dSndSourceMgr_c::GetInstance()->setup();
    EGG::AudioRmtSpeakerMgr::setup(0, nullptr);
    mIsInitialized = true;
}

void dSndMgr_c::initHbm(u32 frame) {
    EGG::AudioUtility::HBM::init(this, restoreEffectsCallback, frame);
}

void dSndMgr_c::calc() {
    if (mIsInitialized) {
        EGG::SimpleAudioMgr::calc();
        EGG::AudioRmtSpeakerMgr::calc();
        dSndPlayerMgr_c::GetInstance()->calc();

        if (dSndPlayerMgr_c::GetInstance()->isInit()) {
            dSndSmallEffectMgr_c::GetInstance()->calcTimer();
            if (!dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_HBM)) {
                dSndControlPlayerMgr_c::GetInstance()->calc();
                if (!dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_CAUTION)) {
                    dSndStateMgr_c::GetInstance()->calc();
                    dSndPlayerMgr_c::GetInstance()->calcActive();
                    dSnd3DManager_c::GetInstance()->calc();
                    dSndSmallEffectMgr_c::GetInstance()->calc();
                    dSndBgmMgr_c::GetInstance()->calc();
                    dSndAreaSoundEffectMgr_c::GetInstance()->calc();
                    dSndHarpSongMgr_c::GetInstance()->calc();
                    dSndSeSoundPool_c::GetInstance()->calc();
                }
            }
        }
    }
}

void dSndMgr_c::restoreEffectsCallback() {
    dSndStateMgr_c::GetInstance()->restoreEffects();
    dSndBgmMgr_c::GetInstance()->restoreEffects();
}
