#include "d/snd/d_snd_small_effect_mgr.h"

#include "common.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_if.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "rvl/OS/OSFastCast.h"
#include "toBeSorted/music_mgrs.h"

SND_DISPOSER_DEFINE(dSndSmallEffectMgr_c)

dSndSmallEffectMgr_c::dSndSmallEffectMgr_c() : mTextboxAdvanceSound(-1), field_0x40(0), field_0x42(0), field_0x44(0) {
    // probably arrays
    field_0x28 = -1;
    field_0x30 = 0;
    field_0x2C = -1;
    field_0x34 = 0;
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
                if (fn_80364DA0(ENEMY_SOUND_MGR)) {
                    fn_80365020(ENEMY_SOUND_MGR);
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
        mTextboxAdvanceSound = dSoundSource_c::getCharacterTalkSoundId(SE_S_TALK_CHAR, source);
        if (mTextboxAdvanceSound == SE_S_TALK_CHAR) {
            resetButtonPressSound();
        }
    }
}
