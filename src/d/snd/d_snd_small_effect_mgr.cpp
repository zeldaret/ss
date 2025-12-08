#include "d/snd/d_snd_small_effect_mgr.h"

#include "common.h"
#include "d/snd/d_snd_area_sound_effect_mgr.h"
#include "d/snd/d_snd_bgm_harp_data.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_bgm_sound.h"
#include "d/snd/d_snd_calc_pitch.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_stage_data.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_global.h"
#include "rvl/OS/OSFastCast.h"

SND_DISPOSER_DEFINE(dSndSmallEffectMgr_c)

dSndSmallEffectMgr_c::dSndSmallEffectMgr_c()
    : field_0x10(0), mTextboxAdvanceSound(-1), field_0x40(0), field_0x42(0), field_0x44(0) {
    for (int i = 0; i < NUM_DELAYED_SOUNDS; i++) {
        mDelayedSoundIds[i] = -1;
        mDelayedSoundTimers[i] = 0;
    }
}

void dSndSmallEffectMgr_c::initialize() {
    resetButtonPressSound();
}

void dSndSmallEffectMgr_c::calc() {
    if (!dSndPlayerMgr_c::GetInstance()->checkFlag(0x4)) {
        for (int i = 0; i < NUM_DELAYED_SOUNDS; i++) {
            if (mDelayedSoundIds[i] != -1) {
                mDelayedSoundTimers[i]--;
                if (mDelayedSoundTimers[i] <= 0) {
                    playSoundInternalChecked(mDelayedSoundIds[i], nullptr);
                    mDelayedSoundIds[i] = -1;
                }
            }
        }
    }
}

void dSndSmallEffectMgr_c::calcTimer() {
    if (field_0x10 <= 0) {
        return;
    }
    field_0x10--;
}

void dSndSmallEffectMgr_c::stopAllSound(s32 fadeFrames) {
    field_0x10 = fadeFrames;
    for (s32 i = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; i <= dSndControlPlayerMgr_c::sPlayerMax; i++) {
        dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(fadeFrames);
    }
    dSndSourceMgr_c::GetInstance()->stopAllSound();
}

void dSndSmallEffectMgr_c::stopAllSoundDemoRelated(s32 fadeFrames) {
    field_0x10 = fadeFrames;
    if (dSndStateMgr_c::GetInstance()->isInDemo()) {
        for (s32 i = dSndPlayerMgr_c::PLAYER_SMALL_NORMAL; i <= dSndControlPlayerMgr_c::sPlayerMax; i++) {
            dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(fadeFrames);
        }
        dSndSourceMgr_c::GetInstance()->stopAllSound();
    } else {
        dSndControlPlayerMgr_c::GetInstance()
            ->getPlayer1(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL)
            ->StopAllSound(fadeFrames);

        for (s32 i = dSndPlayerMgr_c::PLAYER_LINK_EQUIPMENT; i <= dSndControlPlayerMgr_c::sPlayerMax; i++) {
            if ((u32)i != dSndPlayerMgr_c::PLAYER_AREA) {
                dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(fadeFrames);
            }
        }
        dSndSourceMgr_c::GetInstance()->stopAllNonPlayerSound();
    }
}

void dSndSmallEffectMgr_c::stopAllSoundExceptEvent(s32 fadeFrames) {
    field_0x10 = fadeFrames;
    for (u32 i = dSndPlayerMgr_c::PLAYER_SMALL_NORMAL; (s32)i <= dSndControlPlayerMgr_c::sPlayerMax; i++) {
        if (i == dSndPlayerMgr_c::PLAYER_LINK_VOICE) {
            dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(20);
        } else if (i != dSndPlayerMgr_c::PLAYER_EVENT) {
            dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(fadeFrames);
        }
    }
    dSndSourceMgr_c::GetInstance()->stopAllSound();
}

void dSndSmallEffectMgr_c::stopAllSoundExceptEffectOrLink(s32 fadeFrames) {
    field_0x10 = fadeFrames;
    for (s32 i = dSndPlayerMgr_c::PLAYER_LINK_EQUIPMENT; i <= dSndControlPlayerMgr_c::sPlayerMax; i++) {
        dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(field_0x10);
    }
    dSndSourceMgr_c::GetInstance()->stopAllNonPlayerSound();
}

bool dSndSmallEffectMgr_c::playSoundInternalChecked(u32 soundId, nw4r::snd::SoundHandle *pHandle) {
    if (dSndStateMgr_c::GetInstance()->getField_0x064()) {
        return false;
    }

    if (field_0x10 > 0) {
        return false;
    }

    if (dSndStateMgr_c::GetInstance()->checkFlag0x10(dSndStateMgr_c::FLAG0x10_0x08)) {
        return false;
    }
    return playSoundInternal(soundId, pHandle);
}

bool dSndSmallEffectMgr_c::playSoundInternal(u32 soundId, nw4r::snd::SoundHandle *pHandle) {
    u32 userParam = dSndPlayerMgr_c::GetInstance()->getSomeUserParam(soundId);
    if (userParam & 2) {
        for (u32 i = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; i <= dSndPlayerMgr_c::PLAYER_EVENT; i++) {
            if (isPlayingSound(i, soundId)) {
                return false;
            }
        }
    } else if (userParam & 1) {
        stopSounds(soundId, 2);
    }

    if (userParam & 0x80000000) {
        return false;
    }

    nw4r::snd::SoundHandle tmpHandle;
    if (pHandle == nullptr) {
        pHandle = &tmpHandle;
    }

    nw4r::snd::SoundStartable::StartResult res = dSndPlayerMgr_c::GetInstance()->startSound(pHandle, soundId, nullptr);
    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        if (userParam & 8) {
            if (EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
                pHandle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_REMOTE_1);
            }
        } else if (userParam & 4) {
            pHandle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_MAIN | nw4r::snd::OUTPUT_LINE_REMOTE_1);
        }

        if (userParam & 0x10 && EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
            u32 rcVariant = dSndPlayerMgr_c::GetInstance()->getRemoConSoundVariant(soundId);
            if (rcVariant != -1) {
                nw4r::snd::SoundHandle rcHandle;
                dSndMgr_c::GetInstance()->getPlayer().StartSound(&rcHandle, rcVariant);
                if (EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
                    rcHandle.SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_REMOTE_1);
                }
            }
        }
    }

    return res == nw4r::snd::SoundStartable::START_SUCCESS;
}

bool dSndSmallEffectMgr_c::playSound(u32 soundId) {
    if (soundId >= SE_S_BUTTON_CALL_L && soundId <= SE_S_PLAY_GUIDE_BUTTON_BLINK) {
        soundId = SE_S_BUTTON_CALL_L;
    } else if (soundId >= SE_S_BUTTON_CALL_R && soundId <= SE_S_2_BUTTON_BLINK) {
        soundId = SE_S_BUTTON_CALL_R;
    } else {
        switch (soundId) {
            case SE_S_BOSS_KEY_TRANS: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BOSS_KEY_TRANS_OFF, 5);
                break;
            }
            case SE_S_BOSS_KEY_TRANS_OFF: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BOSS_KEY_TRANS, 5);
                break;
            }
            case SE_S_HP_GAUGE_UP: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HEART_PIECE_GET)) {
                    return false;
                }
                break;
            }
            case SE_S_HEART_PIECE_GET: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HP_GAUGE_UP)) {
                    return false;
                }
                break;
            }
            case SE_S_MAP_OPEN:
            case SE_S_MENU_IN:  {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HELP_OUT, 1);
                break;
            }
            case SE_S_HELP_OUT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_IN) ||
                    isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MAP_OPEN)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_SELECT_TURN_PAGE_LEFT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_LEFT)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_SELECT_TURN_PAGE_RIGHT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_RIGHT)) {
                    return false;
                }
                break;
            }
            case SE_S_ITEM_SELECT_START: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BM_KEEP_ERROR)) {
                    soundId = SE_S_ITEM_SELECT_START_WAIT;
                }
                break;
            }
            case SE_S_SKIP: {
                if (dSndStateMgr_c::GetInstance()->isInEvent()) {
                    dSndStateMgr_c::GetInstance()->onSkipEvent();
                }
                break;
            }
            case SE_S_WINDOW_PRESS_A: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT, SE_S_POINTER_OK)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_P1_HOLD_POINTER: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_P1_POINT_ITEM, 0);
                break;
            }
            case SE_S_MENU_P1_POINT_ITEM: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_P1_HOLD_POINTER)) {
                    return false;
                }
                break;
            }
            case SE_S_FIRST_PERSON_ON:
            case SE_S_FIRST_PERSON_OFF: {
                if (!isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_DOWSING_SELECT_START)) {
                    playSound(SE_S_DOWSING_SELECT_START);
                }
                break;
            }
            case SE_S_DOWSING_WAIT:
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_FIRST_PERSON_ON)) {
                    return false;
                }
                break;
        }
    }

    return playSoundInternal(soundId);
}

bool dSndSmallEffectMgr_c::playSoundWithPan(u32 soundId, f32 pan) {
    bool ok = playSoundInternal(soundId);
    if (ok) {
        if (pan > 1.0f) {
            pan = 1.0f;
        } else if (pan < -1.0f) {
            pan = -1.0f;
        }
        mNormalSound.SetPan(pan);
    }
    return ok;
}

bool dSndSmallEffectMgr_c::doSideEffects(u32 soundId) {
    if (soundId == SE_S_FS2_SELECT_START || soundId == SE_S_NE_SELECT_OK) {
        dSndBgmMgr_c::GetInstance()->stopAllBgm(15);
        dSndAreaSoundEffectMgr_c::GetInstance()->stopSounds(15);
    }

    u32 userParam = dSndPlayerMgr_c::GetInstance()->getSomeUserParam(soundId);
    if ((userParam & 0x80) && dSndStateMgr_c::GetInstance()->isInEvent()) {
        return false;
    }

    if (userParam & 2) {
        for (u32 i = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; i <= dSndPlayerMgr_c::PLAYER_SMALL_NORMAL; i++) {
            if (isPlayingSound(i, soundId)) {
                return false;
            }
        }
    } else if (userParam & 1) {
        stopSounds(soundId, 2);
    }
    return true;
}

bool dSndSmallEffectMgr_c::playSoundInternal(u32 soundId) {
    u32 userParam = dSndPlayerMgr_c::GetInstance()->getSomeUserParam(soundId);

    // Why is this flag weird
    if ((userParam >> 31) != 0) {
        return false;
    }

    // Bunch of code duplication between doSideEffects and playSoundInternal
    if (doSideEffects(soundId)) {
        return playSoundInternal(soundId, &mNormalSound);
    }
    return false;
}

nw4r::snd::SoundHandle *dSndSmallEffectMgr_c::getHoldSoundHandle(u32 soundId) {
    // Find an existing handle holding this sound
    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        if (h->GetId() == soundId) {
            return h;
        }
    }

    // Find a free handle
    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        if (!h->IsAttachedSound()) {
            return h;
        }
    }

    // Drop a lower-priority sound
    nw4r::snd::SoundHandle *least = nullptr;
    nw4r::snd::SoundArchive::SoundInfo info;
    dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(soundId, &info);
    s32 newPriority = info.playerPriority;

    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(h->GetId(), &info);
        if (info.playerPriority < newPriority) {
            newPriority = info.playerPriority;
            least = h;
        }
    }
    return least;
}

bool dSndSmallEffectMgr_c::holdSound(u32 soundId) {
    nw4r::snd::SoundHandle *h = getHoldSoundHandle(soundId);
    if (h != nullptr) {
        return holdSound(soundId, h);
    }
    return false;
}

bool dSndSmallEffectMgr_c::holdSoundWithPitch(u32 soundId, f32 pitch) {
    nw4r::snd::SoundHandle *h = getHoldSoundHandle(soundId);
    bool ok = false;
    if (h != nullptr) {
        ok = holdSound(soundId, h);
    }
    if (ok && soundId == SE_S_GAUGE_SHIELD_UP_LV) {
        f32 actualPitch = pitch + 1.0f;
        if (actualPitch > 2.0f) {
            actualPitch = 2.0f;
        }
        h->SetPitch(actualPitch);
    }
    return ok;
}

bool dSndSmallEffectMgr_c::holdSound(u32 soundId, nw4r::snd::SoundHandle *handle) {
    if (handle == nullptr) {
        return false;
    }
    u32 param = dSndPlayerMgr_c::GetInstance()->getSomeUserParam(soundId);
    if ((param & 0x80000000) != 0) {
        return false;
    }
    nw4r::snd::SoundStartable::StartResult result =
        dSndMgr_c::GetInstance()->getPlayer().HoldSoundReturnStatus(handle, soundId, nullptr);
    if (result == nw4r::snd::SoundStartable::START_SUCCESS) {
        if ((param & 0x8) != 0) {
            if (EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
                handle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_REMOTE_1);
            }
        } else if ((param & 0x4) != 0) {
            handle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_MAIN | nw4r::snd::OUTPUT_LINE_REMOTE_1);
        }
    }

    return result == nw4r::snd::SoundStartable::START_SUCCESS;
}

bool dSndSmallEffectMgr_c::playSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) {
    return dSndDistantSoundActorPool_c::GetInstance()->startSound(soundId, position);
}

bool dSndSmallEffectMgr_c::playSoundAtPosition2(u32 soundId, const nw4r::math::VEC3 *position) {
    return dSndDistantSoundActorPool_c::GetInstance()->startSound(soundId, position);
}

bool dSndSmallEffectMgr_c::holdBowChargeSound(f32 remainingChargeAmount) {
    nw4r::snd::SoundHandle *pHandle = getHoldSoundHandle(SE_S_BW_FOCUS_LV);
    bool ok = false;
    if (pHandle != nullptr) {
        ok = holdSound(SE_S_BW_FOCUS_LV, pHandle);
    }
    if (ok) {
        f32 chargeProgress = 1.0f - remainingChargeAmount;
        if (chargeProgress < 0.0f) {
            chargeProgress = 0.0f;
        }

        // Bow charge sound increases in volume as it's charging up
        f32 volume = chargeProgress / 2.0f;
        volume += 0.5f;
        if (volume > 1.0f) {
            volume = 1.0f;
        }
        pHandle->SetVolume(volume, 0);

        // Bow charge sound pitches up by 25% until it's complete
        f32 pitch = chargeProgress / 4.0f + 1.0f;
        pHandle->SetPitch(pitch);
    }
    return ok;
}

bool dSndSmallEffectMgr_c::holdFinisherPromptSound(const nw4r::math::VEC3 *position) {
    if (dSndStateMgr_c::GetInstance()->isInEvent()) {
        return false;
    }
    return dSndDistantSoundActorPool_c::GetInstance()->holdSound(SE_S_FOCUS_FINISHER_LV, position);
}

bool dSndSmallEffectMgr_c::playDowsingPingSound(f32 volume, f32 pitch) {
    bool result = playSoundInternal(SE_S_DOWSING_SOUND, &mDowsingSoundHandle);
    if (result) {
        mDowsingSoundHandle.SetVolume(volume, 0);
        if (pitch < 0.8408964f) {
            pitch = 0.8408964f;
        } else if (pitch < 1.0594631f) {
            pitch = 1.0f;
        } else if (pitch < 1.122462f) {
            pitch = 1.0594631f;
        } else if (pitch < 1.1892071f) {
            pitch = 1.122462f;
        } else if (pitch < 1.2599211f) {
            pitch = 1.1892071f;
        } else if (pitch < 1.3348398f) {
            pitch = 1.2599211f;
        } else if (pitch < 1.4142135f) {
            pitch = 1.3348398f;
        } else if (pitch < 1.4983071f) {
            pitch = 1.4142135f; // sqrt(2)
        } else if (pitch > 1.4983071f) {
            pitch = 1.4983071f;
        }
        mDowsingSoundHandle.SetPitch(pitch);
    }

    return result;
}

bool dSndSmallEffectMgr_c::holdDowsingNearestSound() {
    return holdSound(SE_S_DOWSING_SOUND_NEAREST, &mDowsingSoundHandle);
}

bool dSndSmallEffectMgr_c::playSirenCountdownSound(s32 timer) {
    bool ok = playSoundInternal(SE_S_SIREN_COUNT_DOWN);
    if (ok) {
        f32 pitch = 1.0f - (timer * 0.01f);
        mNormalSound.SetPitch(pitch);
    }
    return ok;
}

bool dSndSmallEffectMgr_c::playMinigameCountdownSound(s32 timer) {
    return playSoundInternal(SE_S_MG_COUNT_DOWN_1);
}

bool dSndSmallEffectMgr_c::playMinigameTimeUpSound(s32 timer) {
    return playSoundInternal(timer <= 3 ? SE_S_MG_TIMEUP_COUNT_02 : SE_S_MG_TIMEUP_COUNT_01);
}

bool dSndSmallEffectMgr_c::playMinigameStartSound() {
    return playSoundInternal(SE_S_MG_START);
}

bool dSndSmallEffectMgr_c::playMinigameFinishSound() {
    dSndBgmMgr_c::GetInstance()->stopAllBgm(10);
    return playSoundInternal(SE_S_MG_FINISH);
}

bool dSndSmallEffectMgr_c::playMinigameFinishWhistleSound() {
    dSndBgmMgr_c::GetInstance()->stopAllBgm(10);
    return playSoundInternal(SE_S_MG_FINISH_WHISTLE);
}

bool dSndSmallEffectMgr_c::playMinigameScoreUpSound(s32 param) {
    u32 soundId = SE_S_MG_SCORE_UP;
    bool ok;
    switch (dSndStateMgr_c::GetInstance()->getStageId_0x044()) {
        case SND_STAGE_F019r:
            // Bamboo Island
            ok = playSoundInternal(SE_S_MG_TAKE_SCORE_UP);
            if (ok) {
                f32 pitch = pitchScaleForPitchIdx(param);
                mNormalSound.SetPitch(pitch);
            }
            return ok;
        case SND_STAGE_F020:
            // Sky Field
            soundId = dSndMgr_c::GetInstance()->getArchive() != nullptr ? (param >> 1) + SE_S_MG_TAKE_SCORE_UP :
                                                                          SE_S_MG_DIVING_SCORE_01;
            break;
        case SND_STAGE_F211:
            // Thrill Digger
            soundId = SE_S_BE_GET_RUPEE;
            break;
        case SND_STAGE_F000:
            // Skyloft (Pumpkin Archery)
            if (param >= 50) {
                soundId = SE_S_MG_PUMP_SHOT_SCORE_UP_50;
            } else if (param >= 40) {
                soundId = SE_S_MG_PUMP_SHOT_SCORE_UP_40;
            } else if (param >= 30) {
                soundId = SE_S_MG_PUMP_SHOT_SCORE_UP_30;
            } else if (param >= 20) {
                soundId = SE_S_MG_PUMP_SHOT_SCORE_UP_20;
            } else {
                soundId = SE_S_MG_PUMP_SHOT_SCORE_UP_10;
            }
            break;
    }
    return playSoundInternal(soundId);
}

bool dSndSmallEffectMgr_c::playMinigameScoreDownSound(s32 score) {
    return playSoundInternal(SE_S_MG_SCORE_DOWN);
}

bool dSndSmallEffectMgr_c::playMinigameMusasabiSound(s32 count) {
    if (count < 0) {
        count = 0;
    } else if (count > 10) {
        count = 10;
    }
    return playSoundInternal(
        dSndMgr_c::GetInstance()->getArchive() != nullptr ? count + SE_S_MG_MUSASABI_01 : SE_S_MG_MUSASABI_01
    );
}

bool dSndSmallEffectMgr_c::playSkbSound(u32 soundId) {
    switch (soundId) {
        case SE_S_SK_POINT:
        case SE_S_SK_INPUT:
            if (isPlayingSound(SE_S_SK_INPUT_DECIDE)) {
                return false;
            }
            break;
        case SE_S_SK_INPUT_DECIDE:
            stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_POINT, 0);
            stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_INPUT, 0);
            break;
        case SE_S_SK_DELETE_ERROR: stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_INPUT, 0); break;
    }
    return playSound(soundId);
}

bool dSndSmallEffectMgr_c::playDemoSound(u32 soundId, nw4r::snd::SoundHandle *pHandle) {
    nw4r::snd::SoundHandle tmpHandle;

    return dSndPlayerMgr_c::GetInstance()->startDemoSound(
               pHandle == nullptr ? &tmpHandle : pHandle, soundId, nullptr
           ) == nw4r::snd::SoundStartable::START_SUCCESS;
}

void dSndSmallEffectMgr_c::stopSounds(u32 playerIdx, u32 soundId, s32 fadeFrames) {
    SoundStopper stopper(soundId, fadeFrames);
    dSndControlPlayerMgr_c::GetInstance()->getPlayer1(playerIdx)->ForEachSound(stopper, false);
}

void dSndSmallEffectMgr_c::stopSounds(u32 soundId, s32 fadeFrames) {
    for (s32 playerId = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; playerId <= dSndControlPlayerMgr_c::sPlayerMax;
         playerId++) {
        stopSounds(playerId, soundId, fadeFrames);
    }
}

bool dSndSmallEffectMgr_c::isPlayingSound(u32 playerIdx, u32 soundId) {
    bool isPlaying = false;
    IsCurrentSoundIdChecker check(soundId, &isPlaying);
    dSndControlPlayerMgr_c::GetInstance()->getPlayer1(playerIdx)->ForEachSound(check, false);
    return isPlaying;
}

bool dSndSmallEffectMgr_c::isPlayingSound(u32 soundId) {
    for (s32 playerId = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; playerId <= dSndControlPlayerMgr_c::sPlayerMax;
         playerId++) {
        if (isPlayingSound(playerId, soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndSmallEffectMgr_c::playButtonPressSoundWhenAdvancingTextBoxes(f32 ratio) {
    if (mTextboxAdvanceSound == -1) {
        return false;
    }

    bool ok = playSound(mTextboxAdvanceSound);
    if (ok) {
        s16 varValue;
        f32 value = ratio * 100.0f;
        OSf32tos16(&value, &varValue);
        nw4r::snd::SeqSoundHandle handle(&mNormalSound);
        handle.WriteVariable(10, varValue);
    }

    return ok;
}

void dSndSmallEffectMgr_c::resetButtonPressSound() {
    mTextboxAdvanceSound = -1;
}

void dSndSmallEffectMgr_c::setButtonPressSound(dSoundSource_c *source) {
    resetButtonPressSound();
    if (source != nullptr) {
        mTextboxAdvanceSound = dSoundSource_c::modifySoundId(SE_S_TALK_CHAR, source);
        if (mTextboxAdvanceSound == SE_S_TALK_CHAR) {
            resetButtonPressSound();
        }
    }
}

bool dSndSmallEffectMgr_c::playBattleHitSound(BattleHitSound_e type, dSoundSource_c *source) {
    // if we're not in battle, don't play any of the hit effects
    if (!dSndBgmMgr_c::GetInstance()->isPlayingAnyBattleMusic()) {
        return false;
    }

    if (source != nullptr) {
        const char *name = source->getName();
        s32 sourceType = source->getSourceType();
        if (sourceType == SND_SOURCE_OBJECT_40) {
            return false;
        }

        switch (type) {
            case BATTLE_TUTTI_GUARDJUST:
                if (sourceType >= SND_SOURCE_BULLET) {
                    // Do not play battle effects for countering bullets
                    return false;
                }
                break;
            case BATTLE_TUTTI_FINISH:
                if (streq(name, "BLasBos")) {
                    // Do not play finish effect for finishing Demise
                    return false;
                }
                break;
            default: break;
        }
    }
    u32 soundId = BGM_BATTLE_TUTTI;
    switch (type) {
        case BATTLE_TUTTI_TURN:      soundId = BGM_BATTLE_TUTTI_TURN; break;
        case BATTLE_TUTTI_JUMP:      soundId = BGM_BATTLE_TUTTI_JUMP; break;
        case BATTLE_TUTTI_FINISH:    soundId = BGM_BATTLE_TUTTI_FINISH; break;
        case BATTLE_TUTTI_GUARDJUST: soundId = BGM_BATTLE_TUTTI_GUARDJUST; break;
        default:                     break;
    }

    if (mBattleTuttiHandle.IsAttachedSound()) {
        u32 alreadyPlayingSound = mBattleTuttiHandle.GetId();
        // BGM_BATTLE_TUTTI_ sounds are ordered by priority apparently
        if (alreadyPlayingSound > soundId) {
            return false;
        }
        if (alreadyPlayingSound == BGM_BATTLE_TUTTI) {
            nw4r::snd::SeqSoundHandle handle(&mBattleTuttiHandle);
            // Do not allow stopping BGM_BATTLE_TUTTI too early
            if ((s32)handle.GetTick() < 12) {
                return false;
            }
        }
        mBattleTuttiHandle.Stop(5);
    }

    dSndBgmSound_c *bgmSound = dSndBgmMgr_c::GetInstance()->getActiveBgmSound();
    if (bgmSound == nullptr) {
        return false;
    }

    dSndBgmDataHarpVarSetBase_c *set = bgmSound->getCurrentHarpVarSet();
    if (set == nullptr) {
        return false;
    }

    bool ok = playSoundInternal(soundId, &mBattleTuttiHandle);
    if (ok) {
        nw4r::snd::SeqSoundHandle seqHandle(&mBattleTuttiHandle);
        if (set->getCount() <= 0) {
            return true;
        }

        s32 count = set->getCount();
        if (count == 1) {
            field_0x40 = set->getVal(0);
            field_0x42 = field_0x40;
            for (int i = 1; i < 4; i++) {
                seqHandle.WriteVariable(i, field_0x40);
            }
            return true;
        } else {
            s32 i2 = 0;
            field_0x44 = 0;
            s32 varIdx = 0;
            u32 mask = 0;

            // Pick a var 0
            if (type == BATTLE_TUTTI_FINISH) {
                field_0x40 = set->getVal(0);
                varIdx = 0;
            } else {
                s32 numHarpBits = set->getNumBitsSet();

                if (numHarpBits <= 0) {
                    // No bits set, just try and pick a random variable
                    // that's not equal to the one we picked before
                    s32 pick = field_0x40;
                    int i = 0;
                    while (pick == field_0x40) {
                        i++;
                        if (i > 0x14) {
                            break;
                        }
                        varIdx = set->getRandomIdx();
                        pick = set->getVal(varIdx);
                    }
                    field_0x40 = pick;
                } else if (numHarpBits == 1) {
                    // One bit set, get the value (using a slightly convoluted method
                    // due to inline complexity) without rejecting it when it equals
                    // the previously picked value
                    varIdx = set->getRandomIdxWithBitSet();
                    field_0x40 = set->getVal(varIdx);
                } else {
                    // Multiple bits set, just pick a random variable with the bit set
                    // that's not equal to the one we picked before
                    s32 pick = field_0x40;
                    int i = 0;
                    while (pick == field_0x40) {
                        i++;
                        if (i > 0x14) {
                            break;
                        }
                        varIdx = set->getRandomIdxWithBitSet();
                        pick = set->getVal(varIdx);
                    }
                    field_0x40 = pick;
                }
            }
            field_0x44 |= 1 << varIdx;
            setBitsIfAdjacent(set, count, field_0x40, &mask);
            seqHandle.WriteVariable(0, field_0x40);

            // Pick a new field_0x42 -> var 1
            u32 mask5 = 0;
            s32 value = 0;
            int i = 0;
            do {
                i++;
                if (i > 0x14) {
                    break;
                }
                u32 rndValue = set->getRandomIdx();
                mask5 = 1 << rndValue;
                value = set->getVal(rndValue);

            } while ((mask & mask5) || value == field_0x42);
            field_0x42 = value;
            field_0x44 |= mask5;

            setBitsIfAdjacent(set, count, field_0x42, &mask);
            seqHandle.WriteVariable(1, field_0x42);

            // Pick new vars 2 and 3
            bool b = field_0x40 != field_0x42;
            s32 seqVarIdx = 2;
            for (; seqVarIdx < 4; seqVarIdx++) {
                u32 rndValue = 0;
                u32 mask5 = 0;
                int i = 0;
                do {
                    i++;
                    if (i > 0x14) {
                        break;
                    }
                    rndValue = set->getRandomIdx();
                    mask5 = 1 << rndValue;
                } while ((mask & mask5) || (seqVarIdx >= 3 && !b && (field_0x44 & mask5)));
                if (!(field_0x44 & mask5)) {
                    b = true;
                }
                s32 nextVal = value = set->getVal(rndValue);
                setBitsIfAdjacent(set, count, nextVal, &mask);
                field_0x44 |= mask5;
                seqHandle.WriteVariable(seqVarIdx, nextVal);
            }
        }
    }

    return ok;
}

void dSndSmallEffectMgr_c::setBitsIfAdjacent(dSndBgmDataHarpVarSetBase_c *set, s32 count, s32 target, u32 *pMask) {
    for (int i = 0; i < count; i++) {
        s32 val = set->getVal(i);
        if (val == target + 1 || val == target - 1) {
            *pMask |= 1 << i;
        }
    }
}
