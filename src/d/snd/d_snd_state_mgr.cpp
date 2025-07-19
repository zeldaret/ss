#include "d/snd/d_snd_state_mgr.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_area_sound_effect_mgr.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_event.h"
#include "d/snd/d_snd_harp_song_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_stage_data.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_sound_area_mgr.h"
#include "egg/core/eggHeap.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/snd/snd_global.h"
#include "sized_string.h"
#include "toBeSorted/event_manager.h"

#include <cstring>

SndEventCallback dSndStateMgr_c::sEventExecuteCallback;
SND_DISPOSER_DEFINE(dSndStateMgr_c);

dSndStateMgr_c::dSndStateMgr_c()
    : field_0x010(0),
      field_0x014(0),
      field_0x018(0),
      field_0x03C(0),
      mStageId(0xAC),
      mPreviousStageId(0xAC),
      mStageTypeFlags(0),
      mLayer(0),
      mRoomId(-1),
      field_0x060(0),
      field_0x064(0),
      field_0x065(false),
      mHasChangedTgSndAreaFlags(false),
      mHasChangedTgSndAreaMgFlags(false),
      mSavedTgSndAreaFlags(-1),
      mSavedTgSndAreaMgFlags(-1),
      mpUnkCallback(nullptr),
      field_0x074(0),
      mpOnEventStartCallback(nullptr),
      field_0x07C(0),
      mpTgSndAreaFlagsChangeCallback(nullptr),
      mpTgSndAreaMgFlagsChangeCallback(nullptr),
      field_0x088(0),
      mSoundEventId(SND_EVENT_0x89),
      mCameraCutCounter(0),
      mEventFlags(0),
      mpStbEventName(nullptr),
      mFrameCounter(0),
      mCameraCutFrameCounter(0),
      mMsgFrameCounter(0),
      mSeLvSoundId(-1),
      field_0x22C(0),
      mpSoundEventDef(nullptr),
      field_0x238(-1),
      field_0x23C(0),
      field_0x23D(0),
      field_0x240(0),
      field_0x244(0),
      field_0x248(0),
      mMsgCounter(-1),
      mMsgWaitSelectCounter(0),
      field_0x254(0),
      field_0x258(0),
      field_0x48C(0),
      mFxSend3DDefault(0.02f),
      mFxSend3DOverride(-1.0f),
      mFxSend3DNext(-1.0f),
      mFxSend3D(0.02f),
      mFxSend3DTarget(0.02f),
      field_0x4A4(-1),
      field_0x4A8(0),
      mNeedsGroupsReload(false) {}

extern "C" void fn_803665B0(nw4r::snd::detail::FxReverbStdParam &, int);

void dSndStateMgr_c::setup(EGG::Heap *pHeap) {
    u32 max = 0;
    for (int i = 0; i < 5; i++) {
        nw4r::snd::detail::FxReverbStdParam param;
        fn_803665B0(param, i);
        mFx.SetParam(param);
        u32 thisSize = mFx.GetRequiredMemSize();
        if (max < thisSize) {
            max = thisSize;
        }
    }
    void *mem = pHeap->alloc(max, 4);
    mFx.AssignWorkBuffer(mem, max);
    resetEventName();
}

void dSndStateMgr_c::onStageOrLayerUpdate() {
    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_CAUTION)) {
        dSndPlayerMgr_c::GetInstance()->leaveCaution();
    }

    bool unk_0x065 = field_0x065;

    onFlag0x10(0x2);
    field_0x064 = 0;

    if (!streq(mStageName, dScGame_c::currentSpawnInfo.getStageName())) {
        mNeedsGroupsReload = true;
        mStageName = dScGame_c::currentSpawnInfo.getStageName();
    }

    if (!mNeedsGroupsReload && dScGame_c::currentSpawnInfo.layer != mLayer) {
        mNeedsGroupsReload = true;
    }

    mLayer = dScGame_c::currentSpawnInfo.layer;
    u32 newStageId = getStageId(dScGame_c::currentSpawnInfo.getStageName(), dScGame_c::currentSpawnInfo.layer);
    mPreviousStageId = mStageId;
    field_0x044 = getStageUnk2(newStageId);
    mStageId = newStageId;

    if (unk_0x065) {
        setCallbacksForStage();
    }
}

inline bool checkStagePrefix(const char *stageName, const char *prefix) {
    return !std::strncmp(prefix, stageName, 2);
}

u32 dSndStateMgr_c::getStageTypeFlags(const char *stageName) const {
    u32 flags = 0;
    if (checkStagePrefix(stageName, "F0")) {
        flags |= STAGE_SKY | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D0")) {
        flags |= STAGE_SKY | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B0")) {
        flags |= STAGE_SKY | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S0")) {
        flags |= STAGE_SKY | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F1")) {
        flags |= STAGE_FOREST | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D1")) {
        flags |= STAGE_FOREST | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B1")) {
        flags |= STAGE_FOREST | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S1")) {
        flags |= STAGE_FOREST | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F2")) {
        flags |= STAGE_MOUNTAIN | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D2")) {
        flags |= STAGE_MOUNTAIN | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B2")) {
        flags |= STAGE_MOUNTAIN | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S2")) {
        flags |= STAGE_MOUNTAIN | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F3")) {
        flags |= STAGE_DESERT | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D3")) {
        flags |= STAGE_DESERT | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B3")) {
        flags |= STAGE_DESERT | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S3")) {
        flags |= STAGE_DESERT | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_BOSS;
    }

    if ((flags & STAGE_SKY) != 0 && std::strstr(stageName, "r") != nullptr) {
        flags |= STAGE_ROOM;
    }

    return flags;
}

bool dSndStateMgr_c::isVolcanicDungeon(u32 stageId) {
    u32 base = getStageUnk2(stageId);
    switch (base) {
        case SND_STAGE_D200:
        case SND_STAGE_D201:
        case SND_STAGE_D201_1:
        case SND_STAGE_D003_0:
        case SND_STAGE_D003_1: return true;
        default:               return false;
    }
}

bool dSndStateMgr_c::isSeekerStoneStage(const char *stageName, s32 layer) {
    if (streq(stageName, "F000") && layer == 22) {
        // Skyloft hint stone
        return true;
    }

    if (streq(stageName, "F202") && layer == 10) {
        // Boko base hint stone
        return true;
    }

    return false;
}

bool dSndStateMgr_c::isSomeSkyloftRoom() const {
    switch (field_0x044) {
        case SND_STAGE_F001r:
        case SND_STAGE_F002r:
        case SND_STAGE_F011r:
        case SND_STAGE_F005r:
        case SND_STAGE_F006r:
        case SND_STAGE_F007r:
        case SND_STAGE_F013r:
        case SND_STAGE_F014r:
        case SND_STAGE_F015r:
        case SND_STAGE_F016r:
        case SND_STAGE_F017r:
        case SND_STAGE_F018r: return true;
        default:              return false;
    }
}

void dSndStateMgr_c::onRestartScene(s32 fadeFrames) {
    if (field_0x064) {
        return;
    }

    s32 nextLayer = dScGame_c::nextSpawnInfo.layer;
    // This combines next stage with current layer, so this effectively checks if you're leaving the Hint Movie scene
    if (isSeekerStoneStage(mStageName, dScGame_c::currentSpawnInfo.layer)) {
        dSndBgmMgr_c::GetInstance()->stopAllBgm(fadeFrames);
    } else if (mLayer != nextLayer) {
        if (isSeekerStoneStage(mStageName, nextLayer)) {
            dSndBgmMgr_c::GetInstance()->stopAllBgm(fadeFrames);
            dSndBgmMgr_c::GetInstance()->prepareBgm(BGM_HINT_SELECT, 0);
            dSndBgmMgr_c::GetInstance()->setOverrideBgmId(BGM_HINT_SELECT);
            dSndBgmMgr_c::GetInstance()->activateOverrideBgmId();
            dSndAreaSoundEffectMgr_c::GetInstance()->stopSounds(fadeFrames);
        } else {
            onGotoStage(fadeFrames);
            onFlag0x10(0x01);
            return;
        }
    }

    dSndSmallEffectMgr_c::GetInstance()->stopAllSoundExceptEvent(fadeFrames);
    dSndBgmMgr_c::GetInstance()->prepareBgm();
    dSndHarpSongMgr_c::GetInstance()->deactivate();
    onFlag0x10(0x01);
    dSndBgmMgr_c::GetInstance()->setField_0x306(1);
    offFlag0x10(0x4);
    offFlag0x10(0x10);
}

void dSndStateMgr_c::loadStageSound() {
    loadStageSound(false);
}

void dSndStateMgr_c::loadStageSound(bool force) {
    if (!force && !mNeedsGroupsReload) {
        return;
    }

    bool demo = isInDemo();
    if (!checkFlag0x18(0x40)) {
        for (int i = 0; i < dSndControlPlayerMgr_c::sNumPlayers; i++) {
            dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(i, 0.0f, 0);
            dSndControlPlayerMgr_c::GetInstance()->setFxSend(i, 0.0f, 0);
        }
    }

    if (checkEventFlag(EVENT_0x800)) {
        if (!dSndBgmMgr_c::GetInstance()->isPlayingBgmSound()) {
            dSndControlPlayerMgr_c::GetInstance()->setupPlayerControlsStage();
            offEventFlag(EVENT_0x800);
        } else {
            dSndControlPlayerMgr_c::GetInstance()->setupPlayerControlsStageBgm();
        }
    } else {
        if (dSndBgmMgr_c::GetInstance()->isPlayingBgmSound()) {
            dSndControlPlayerMgr_c::GetInstance()->setupPlayerControlsStageBgm();
            onEventFlag(EVENT_0x800);
        } else {
            dSndControlPlayerMgr_c::GetInstance()->setupPlayerControlsStage();
        }
    }

    dSndControlPlayerMgr_c::GetInstance()->muteScenePlayers(0);
    dSndPlayerMgr_c::GetInstance()->shutdownDemo();
    field_0x4A8 = 1;
    bool bHasState1 = false;

    if (isSomeSkyloftRoom()) {
        u32 id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId("GRP_FAN_TIMECHANGE");
        if (dSndMgr_c::GetInstance()->getPlayer().IsLoadedGroup(id)) {
            bHasState1 = true;
        } else {
            dSndPlayerMgr_c::GetInstance()->popToState0();
            if (dSndMgr_c::GetInstance()->loadGroup(id, nullptr, 0)) {
                dSndPlayerMgr_c::GetInstance()->saveState1();
                bHasState1 = true;
            }
        }
    }

    const dSndStageInfo *info = &dSndStageInfo::sInfos[mStageId];

    u32 id = -1;
    int round = 1;

    bool b1 = false;
    SizedString<64> label;

    if (info->unk4 == mStageId) {
        const char *stageName = getStageName(info->unk4);
        label.sprintf("GRP_A_%s_L%d", stageName, mLayer);
        round = 2;
    } else {
        const char *stageName = getCurrentStageName4();
        label.sprintf("GRP_A_%s", stageName);
    }

    for (; round > 0; round--) {
        id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId(label);
        if (field_0x4A4 == id && field_0x4A4 != -1) {
            b1 = true;
            break;
        }

        if (id != -1) {
            if (bHasState1) {
                dSndPlayerMgr_c::GetInstance()->popToState1();
            } else {
                dSndPlayerMgr_c::GetInstance()->popToState0();
            }
            if (dSndMgr_c::GetInstance()->loadGroup(id, nullptr, 0)) {
                field_0x4A4 = id;
                dSndPlayerMgr_c::GetInstance()->saveState2();
                break;
            }
        }

        if (round == 2) {
            // we tried layer variant, now try variant without layer
            const char *stageName = getCurrentStageName4();
            label.sprintf("GRP_A_%s", stageName);
        }
    }

    if (b1) {
        dSndPlayerMgr_c::GetInstance()->popToState2();
    } else if (id == -1) {
        if (bHasState1) {
            dSndPlayerMgr_c::GetInstance()->popToState1();
        } else {
            dSndPlayerMgr_c::GetInstance()->popToState0();
        }
        dSndPlayerMgr_c::GetInstance()->saveState2();
        field_0x4A4 = -1;
    }

    if (!demo && (field_0x03C != 2 || field_0x065)) {
        if (field_0x065) {
            id = GRP_SIREN;
        } else if (checkStageTypeFlag(STAGE_ROOM)) {
            id = GRP_INDOOR;
        } else if (checkStageTypeFlag(STAGE_FIELD)) {
            id = GRP_FIELD;
        } else if (checkStageTypeFlag(STAGE_DUNGEON)) {
            if (isVolcanicDungeon(mStageId)) {
                id = GRP_DUNGEON_BATTLE_ROOM;
            } else {
                id = GRP_DUNGEON;
            }
        } else if (checkStageTypeFlag(STAGE_BOSS)) {
            id = GRP_BOSS;
        } else {
            id = GRP_DEFAULT;
        }

        if (id != -1) {
            dSndMgr_c::GetInstance()->loadGroup(id, nullptr, 0);
        }
    }

    if (!field_0x065) {
        id = -1;
        if (!demo) {
            if (field_0x03C == 2) {
                id = GRP_SKY;
            } else if (checkStageTypeFlag(STAGE_FOREST)) {
                id = GRP_FOREST;
            } else if (checkStageTypeFlag(STAGE_MOUNTAIN)) {
                id = GRP_MOUNTAIN;
            } else if (checkStageTypeFlag(STAGE_DESERT)) {
                id = GRP_DESERT;
            }

            if (id != -1) {
                dSndMgr_c::GetInstance()->loadGroup(id, nullptr, 0);
            }
        }

        SizedString<64> label;
        label.sprintf("GRP_%s_L%d", &mStageName, mLayer);
        id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId(label);
        if (id == -1) {
            label.sprintf("GRP_%s", &mStageName);
            id = dSndMgr_c::GetInstance()->getArchive()->ConvertLabelStringToGroupId(label);
        }

        if (id != -1) {
            dSndMgr_c::GetInstance()->loadGroup(id, nullptr, 0);
        }

        if (demo) {
            // REGSWAP
            if (dSndPlayerMgr_c::GetInstance()->loadDemoArchive(getCurrentStageMusicDemoName())) {
                for (u32 i = dSndPlayerMgr_c::PLAYER_LINK_BODY; i <= dSndPlayerMgr_c::PLAYER_EVENT; i++) {
                    dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->StopAllSound(0);
                }
                dSndControlPlayerMgr_c::GetInstance()->setupPlayerControlsDemo();
            }
        }
    }

    field_0x4A8 = 0;
    dSndBgmMgr_c::GetInstance()->loadStageSound();
    f32 volume = -1.0f;
    dSndAreaSoundEffectMgr_c::GetInstance()->loadStageSound(&volume);
    if (volume > 0.0f) {
        mFxSend3DDefault = volume;
    }
}

void dSndStateMgr_c::calcEvent() {
    if (!isInEvent()) {
        return;
    }

    bool b = false;
    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_CAUTION) ||
        dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_MAP) ||
        dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_MSG_WAIT)) {
        b = true;
    }

    if (mpSoundEventDef != nullptr && sEventExecuteCallback != nullptr) {
        (sEventExecuteCallback)();
    }

    if (!b) {
        if (EventManager::getCurrentEventName() == nullptr) {
            if (!checkEventFlag(EVENT_0x800)) {
                endEvent(false);
            }
        } else {
            if (mSeLvSoundId != -1) {
                dSndMgr_c::GetInstance()->holdSound(&mSeLvSoundHandle, mSeLvSoundId);
            }
            calcBgm();
            calcSe();
            calcCmd();
            mFrameCounter++;
            mCameraCutFrameCounter++;
            mMsgFrameCounter++;
            nw4r::snd::SeqSoundHandle::WriteGlobalVariable(2, mCameraCutFrameCounter);
        }
    }
}

void dSndStateMgr_c::setEvent(const char *eventName) {
    if (eventName == nullptr) {
        return;
    }

    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_CAUTION)) {
        dSndPlayerMgr_c::GetInstance()->leaveCaution();
    }

    if (field_0x064 == 0) {
        if (streq(mEventName, "DefaultSkip")) {
            onSkipEvent();
            mPrevEventName = mEventName;
            return;
        }

        if (!checkFlag0x18(0x20) || !checkFlag0x10(0x04)) {
            SizedString<64> prevEvent = mEventName;
            if (isInEvent()) {
                field_0x23C = 1;
                finalizeEvent(false);
            } else {
                field_0x23C = 0;
            }

            // TODO - constness or direct access
            if (checkEventFlag(EVENT_0x04)) {
                offEventFlag(EVENT_0x04);
            }
            mEventName = eventName;
            mPrevEventName = prevEvent;
            resetEventVars();
            field_0x23D = 0;
            field_0x238 = -1;

            if (streq(mEventName, "STB")) {
                if (mpStbEventName != nullptr) {
                    mEventName = mpStbEventName;
                }
                onEventFlag(EVENT_DEMO);
            } else {
                mpStbEventName = nullptr;
            }

            if (!checkFlag0x10(0x04)) {
                if (mpUnkCallback != nullptr) {
                    mpUnkCallback();
                }
                field_0x23D = 1;
                dSndBgmMgr_c::GetInstance()->setField_0x306(1);
                dSndBgmMgr_c::GetInstance()->prepareBgm();
                onEventFlag(EVENT_0x04);
                onFlag0x10(0x04);
                dSndControlPlayerMgr_c::GetInstance()->unmuteScenePlayers(30);
            }
            bool b2 = checkEventFlag(EVENT_0x800) && dSndBgmMgr_c::GetInstance()->isPlayingBgmSound();
            if (checkEventFlag(EVENT_0x800) && field_0x064 == 0) {
                // TODO - constness or direct access
                offEventFlag(0x800);
            }

            mSoundEventId = SND_EVENT_0x89;
            initializeEventCallbacks(mEventName);
            s32 i3 = 0;
            SizedString<64> eventLabel;
            if (mSoundEventId == SND_EVENT_JMAP) {
                s32 eventRoomId = EventManager::getCurrentEventRoomId();
                // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
                eventLabel.sprintf("%s_R%d_JMAP_%d", mStageName, mRoomId, eventRoomId);
            } else if (mSoundEventId == SND_EVENT_JMAPAllMove) {
                s32 eventRoomId = EventManager::getCurrentEventRoomId();
                // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
                eventLabel.sprintf("%s_R%d_JMAPAllMove_%d", mStageName, mRoomId, eventRoomId);
            } else {
                eventLabel.sprintf("%s", &mEventName);
            }

            if (checkEventFlag(EVENT_DEMO)) {
                mBgmName.sprintf("BGM_%s", &eventLabel);
                mFanName.sprintf("FAN_%s", &eventLabel);
                mSeName.sprintf("SE_%s", &eventLabel);
                mCmdName.sprintf("CMD_%s", &eventLabel);
            } else {
                mBgmName.sprintf("BGM_EVENT_%s", &eventLabel);
                mFanName.sprintf("FAN_EVENT_%s", &eventLabel);
                mSeName.sprintf("SE_EVENT_%s", &eventLabel);
                mCmdName.sprintf("CMD_EVENT_%s", &eventLabel);
            }

            if (checkEventFlag(EVENT_0x04)) {
                field_0x238 = getBgmLabelSoundId();
                if (b2 && field_0x238 == -1) {
                    field_0x23D = 0;
                }
            } else {
                i3 = playFanOrBgm(getBgmLabelSoundId());
                if (i3) {
                    field_0x23D = 0;
                }
            }

            handleFan();
            handleSe();
            handleSeLv();
            handleCmd();

            if (mSoundEventId != SND_EVENT_0x89) {
                onEventFlag(EVENT_IN_EVENT);
                mFrameCounter = 0;
                mCameraCutFrameCounter = 0;
                mMsgFrameCounter = 0;
                if (mpOnEventStartCallback != nullptr) {
                    (mpOnEventStartCallback)(mSoundEventId, mEventFlags);
                }
                if (field_0x238 == -1) {
                    dSndBgmMgr_c::GetInstance()->onEventStart(mEventName, mSoundEventId, i3);
                }
                dSndAreaSoundEffectMgr_c::GetInstance()->onEventStart();
                dSndSourceMgr_c::GetInstance()->onEventStart();
            }
        }
    }
}

bool dSndStateMgr_c::isInStage(const char *stageName) {
    return !std::strcmp(dScGame_c::currentSpawnInfo.getStageName(), stageName);
}

void dSndStateMgr_c::initializeEventCallbacks(const char *name) {
    if (name == nullptr) {
        return;
    }

    mpSoundEventDef = nullptr;
    sEventExecuteCallback = nullptr;
    resetOverrides();

    if (checkEventFlag(EVENT_DEMO)) {
        handleDemoEvent(name);
        return;
    }

    if (mSoundEventId == SND_EVENT_0x87) {
        return;
    }

    if (!handleStageEvent(name)) {
        handleGlobalEvent(name);
    }

    if (mSoundEventId == SND_EVENT_0x89) {
        if (strstr(name, "Intro")) {
            mSoundEventId = SND_EVENT_0x88;
        } else {
            mSoundEventId = SND_EVENT_0x86;
            cbUnkNoop();
        }
    }
}

void dSndStateMgr_c::doLabelSuffix(const char *suffix) {
    SizedString<64> label;
    label.sprintf("%s%s", &mBgmName, suffix);
    doBgm(label);
    label.sprintf("%s%s", &mSeName, suffix);
    doSe(label);
    label.sprintf("%s%s", &mCmdName, suffix);
    doCmd(label);
}

u32 dSndStateMgr_c::getSeCameraId() {
    SizedString<64> label;
    label.sprintf(getCameraFmt(), &mSeName, mCameraCutCounter, mCameraCutFrameCounter);
    return convertSeLabelToSoundId(label);
}

u32 dSndStateMgr_c::getSeMsgWaitId() {
    SizedString<64> label;
    label.sprintf(getMsgWaitFmt(), &mSeName, mMsgCounter, mMsgWaitSelectCounter, mMsgFrameCounter);
    return convertSeLabelToSoundId(label);
}

u32 dSndStateMgr_c::getSeFrameCountId() {
    SizedString<64> label;
    label.sprintf(getFrameFmt(), &mSeName, mFrameCounter);
    return convertSeLabelToSoundId(label);
}

u32 dSndStateMgr_c::getBgmCameraId() {
    SizedString<64> label;
    label.sprintf(getCameraFmt(), &mBgmName, mCameraCutCounter, mCameraCutFrameCounter);
    return convertBgmLabelToSoundId(label);
}

u32 dSndStateMgr_c::getBgmMsgWaitId() {
    SizedString<64> label;
    label.sprintf(getMsgWaitFmt(), &mBgmName, mMsgCounter, mMsgWaitSelectCounter, mMsgFrameCounter);
    return convertBgmLabelToSoundId(label);
}

u32 dSndStateMgr_c::getBgmFrameCountId() {
    SizedString<64> label;
    label.sprintf(getFrameFmt(), &mBgmName, mFrameCounter);
    return convertBgmLabelToSoundId(label);
}

u32 dSndStateMgr_c::getCmdCameraId() {
    SizedString<64> label;
    label.sprintf(getCameraFmt(), &mCmdName, mCameraCutCounter, mCameraCutFrameCounter);
    return convertCmdLabelToSoundId(label);
}

u32 dSndStateMgr_c::getCmdMsgWaitId() {
    SizedString<64> label;
    label.sprintf(getMsgWaitFmt(), &mCmdName, mMsgCounter, mMsgWaitSelectCounter, mMsgFrameCounter);
    return convertCmdLabelToSoundId(label);
}

u32 dSndStateMgr_c::getCmdFrameCountId() {
    SizedString<64> label;
    label.sprintf(getFrameFmt(), &mCmdName, mFrameCounter);
    return convertCmdLabelToSoundId(label);
}

void dSndStateMgr_c::clearEventExecuteCallback() {
    sEventExecuteCallback = nullptr;
}

bool dSndStateMgr_c::calcBgm() {
    bool ok = playFanOrBgm(getBgmCameraId());
    if (!ok) {
        ok = playFanOrBgm(getBgmMsgWaitId());
    }
    if (!ok) {
        // @bug (?) shouldn't this assign to ok? Maybe a shadowing redeclaration...
        (void)playFanOrBgm(getBgmFrameCountId());
    }
    return ok;
}

void dSndStateMgr_c::calcSe() {
    playSe(getSeCameraId());
    playSe(getSeMsgWaitId());
    playSe(getSeFrameCountId());
}

void dSndStateMgr_c::handleSeLv() {
    SizedString<0x40> name = mSeName;
    name += "_LV";
    u32 soundId = convertSeLabelToSoundId(name);
    if (soundId != -1) {
        mSeLvSoundId = soundId;
        if (dSndMgr_c::GetInstance()->holdSound(&mSeLvSoundHandle, soundId)) {
            u32 id = dSndPlayerMgr_c::GetInstance()->getDemoArchive()->GetSoundUserParam(soundId);
            if ((dSndPlayerMgr_c::sEventMuteFlagsMask & id & 0x2000000) == 0) {
                mSeLvSoundHandle.SetFxSend(nw4r::snd::AUX_A, mFxSend3D);
            }
        }
    } else {
        mSeLvSoundId = -1;
    }
}

void dSndStateMgr_c::calcCmd() {
    playCmd(getCmdCameraId());
    playCmd(getCmdMsgWaitId());
    playCmd(getCmdFrameCountId());
}

bool dSndStateMgr_c::finalizeEvent(bool skipped) {
    dSndPlayerMgr_c *mgr = dSndPlayerMgr_c::GetInstance();
    if (mgr->checkFlag(dSndPlayerMgr_c::MGR_CAUTION)) {
        mgr->leaveCaution();
    }
    if (!isInEvent()) {
        return false;
    }

    if (mpSoundEventDef != nullptr && mpSoundEventDef->finalizeCb) {
        mpSoundEventDef->finalizeCb(skipped);
    }

    if (!isInDemo()) {
        nw4r::snd::SoundPlayer *p = dSndControlPlayerMgr_c::GetInstance()->getPlayer1(dSndPlayerMgr_c::PLAYER_EVENT);
        SoundStopperIfParamFlag20 stopper;
        p->ForEachSound(stopper, false);
    }

    SizedString<64> label;
    label.sprintf("%s_FIN", &mSeName);
    playSe(dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label));
    label.sprintf("%s_FIN", &mBgmName);
    doBgm(label);
    label.sprintf("%s_FIN", &mCmdName);
    doCmd(label);

    return dSndBgmMgr_c::GetInstance()->onEventFinalize(mEventName, mSoundEventId, skipped);
}

// TODO - the whole "camera cut" thing seems plausible but I haven't confirmed it yet
void dSndStateMgr_c::onCameraCut(s32 cutIdx) {
    if (cutIdx <= 0) {
        mCameraCutCounter++;
    } else {
        mCameraCutCounter = cutIdx;
    }
    mCameraCutFrameCounter = 0;
    nw4r::snd::SeqSoundHandle::WriteGlobalVariable(1, mCameraCutCounter);
    nw4r::snd::SeqSoundHandle::WriteGlobalVariable(2, 0);
    SizedString<64> suffix;
    suffix.sprintf("_C%d", mCameraCutCounter);
    doLabelSuffix(suffix);
}

bool dSndStateMgr_c::isInEvent() {
    return checkEventFlag(EVENT_IN_EVENT);
}

bool dSndStateMgr_c::isInEvent(const char *eventName) {
    return streq(mEventName, eventName);
}

void dSndStateMgr_c::onMsgStart(s32 idx) {
    if (idx >= 0) {
        mMsgCounter = idx;
    } else {
        mMsgCounter++;
    }
    field_0x254 = 1;
    mMsgWaitSelectCounter = 0;
    mMsgFrameCounter = 0;
    SizedString<64> label;
    label.sprintf("_MS%d", mMsgCounter);
    doLabelSuffix(label);
}

void dSndStateMgr_c::onMsgEnd() {
    field_0x254 = 0;
    SizedString<64> label;
    label.sprintf("_ME%d", mMsgCounter);
    doLabelSuffix(label);
}

void dSndStateMgr_c::onMsgWaitStart() {
    mMsgFrameCounter = 0;
    mMsgWaitSelectCounter++;

    if (mpSoundEventDef != nullptr && mpSoundEventDef->msgWaitStartCb != nullptr) {
        mpSoundEventDef->msgWaitStartCb(mMsgWaitSelectCounter);
    }

    SizedString<64> label;
    label.sprintf("_M%d_WS%d", mMsgCounter, mMsgWaitSelectCounter);
    doLabelSuffix(label);
}

void dSndStateMgr_c::onMsgWaitEnd() {
    if (mpSoundEventDef != nullptr && mpSoundEventDef->msgWaitEndCb != nullptr) {
        mpSoundEventDef->msgWaitEndCb(mMsgWaitSelectCounter);
    }

    SizedString<64> label;
    label.sprintf("_M%d_WE%d", mMsgCounter, mMsgWaitSelectCounter);
    doLabelSuffix(label);
}

void dSndStateMgr_c::onLinkDie() {
    if (!checkFlag0x10(0x8)) {
        onFlag0x10(0x8);
        dSndBgmMgr_c::GetInstance()->stopAllBgm(15);
        dSndSmallEffectMgr_c::GetInstance()->stopAllSoundExceptEffectOrLink(15);
        if (field_0x065 == 0) {
            dSndBgmMgr_c::GetInstance()->prepareFanSound(FAN_GAMEOVER);
        }
    }
}

void dSndStateMgr_c::setStbEventName(const char *eventName) {
    mpStbEventName = eventName;
}

void dSndStateMgr_c::calcRoomId() {
    if (checkFlag0x10(4)) {
        if (field_0x060 > 0) {
            field_0x060--;
        } else {
            if (mRoomId == -1) {
                field_0x060 = 30;
            }
            s32 newRoom = dSndSourceMgr_c::GetInstance()->getPlayerSourceRoomId();
            if (mRoomId != newRoom) {
                setRoomId(newRoom);
            } else if (newRoom != -1 && !checkFlag0x10(0x10)) {
                setRoomId(newRoom);
            }
        }
    }
}

void dSndStateMgr_c::setRoomId(s32 roomId) {
    // ...
}

void dSndStateMgr_c::calcTgSnd() {
    mHasChangedTgSndAreaFlags = false;
    mHasChangedTgSndAreaMgFlags = false;
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link != nullptr && dTgSndMg_c::GetInstance() != nullptr) {
        u32 flags = dTgSndMg_c::GetInstance()->getSndFlags();
        if (flags != mSavedTgSndAreaMgFlags) {
            mHasChangedTgSndAreaMgFlags = true;
            if (mpTgSndAreaMgFlagsChangeCallback != nullptr) {
                (mpTgSndAreaMgFlagsChangeCallback)(flags, mSavedTgSndAreaMgFlags);
            }
            dSndBgmMgr_c::GetInstance()->onTgSndAreaMgFlagsChange(flags, mSavedTgSndAreaMgFlags);
            mSavedTgSndAreaMgFlags = flags;
        }

        if (dSndSourceMgr_c::getBoomerangSource() != nullptr) {
            flags = mSavedTgSndAreaMgFlags;
        } else {
            flags = link->mTgSndAreaFlags;
        }
        if (flags != mSavedTgSndAreaFlags) {
            mHasChangedTgSndAreaFlags = true;
            if (mpTgSndAreaFlagsChangeCallback != nullptr) {
                (mpTgSndAreaFlagsChangeCallback)(flags, mSavedTgSndAreaFlags);
            }
            mFxSend3DNext = getUserParamVolume(flags);
            dSndBgmMgr_c::GetInstance()->onTgSndAreaFlagsChange(flags, mSavedTgSndAreaFlags);
            mSavedTgSndAreaFlags = flags;
        }
    }
}

void dSndStateMgr_c::calcFilters() {
    dCamera_c *cam = dScGame_c::getCamera(0);
    if (cam != nullptr && !checkFlag0x258(0x2)) {
        if (cam->isUnderwater()) {
            if (!checkFlag0x258(0x1)) {
                resetFlag0x258();
                onFlag0x258(1);
                setFiltersIfUnderwater();
            }
            f32 depth = cam->getUnderwaterDepth();
            dSndAreaSoundEffectMgr_c::GetInstance()->holdInWaterLvSound(depth);
            f32 maxDepth = isInStage("F103") ? 5000.0f : 1000.0f;
            f32 unk = isInStage("F103") ? 0.5f : 0.3f;
            if (depth < maxDepth) {
                f32 tmp = (1.0f - unk);
                f32 tmp2 = (depth / maxDepth);
                unk = 1.0f - tmp2 * tmp;
            }
            dSndBgmMgr_c::GetInstance()->updateField_0x2F4(unk);
        } else {
            if (checkFlag0x258(0x1)) {
                offFlag0x258(1);
                resetLpfAndFxSend();
            }

            if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
                if (!checkFlag0x258(0x4)) {
                    resetFlag0x258();
                    onFlag0x258(0x4);
                    setBgmAndStageEffectLpf();
                    dSndBgmMgr_c::GetInstance()->setField_0x307(0);
                }
            } else {
                if (checkFlag0x258(0x4)) {
                    offFlag0x258(0x4);
                    resetBgmAndStageEffectLpf();
                    if (dSndBgmMgr_c::GetInstance()->getField_0x308() != 0) {
                        dSndBgmMgr_c::GetInstance()->setField_0x307(1);
                    }
                }
            }
        }
    }
}

void dSndStateMgr_c::setFiltersIfUnderwater() {
    dCamera_c *cam = dScGame_c::getCamera(0);
    if (cam == nullptr || cam->isUnderwater()) {
        setBgmLpfAndFxSendIfUnderwater();
        for (u32 id = dSndPlayerMgr_c::PLAYER_LINK_BODY; id < dSndPlayerMgr_c::PLAYER_LINK_EQUIPMENT + 1; id++) {
            dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, -0.45f, 15);
        }

        for (u32 id = dSndPlayerMgr_c::PLAYER_ENEMY; id < dSndPlayerMgr_c::PLAYER_AREA + 1; id++) {
            dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, -0.41f, 15);
        }

        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV, -0.3f, 15);

        for (u32 id = dSndPlayerMgr_c::PLAYER_ENEMY; id <= dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV; id++) {
            dSndControlPlayerMgr_c::GetInstance()->setFxSend(id, 0.14f, 15);
        }
    }
}

void dSndStateMgr_c::setBgmLpfAndFxSendIfUnderwater() {
    dCamera_c *cam = dScGame_c::getCamera(0);
    if (cam == nullptr || cam->isUnderwater()) {
        for (u32 id = dSndPlayerMgr_c::PLAYER_BGM; id < dSndPlayerMgr_c::PLAYER_BGM_BATTLE + 1; id++) {
            dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, -0.55f, 15);
            dSndControlPlayerMgr_c::GetInstance()->setFxSend(id, 0.2f, 15);
        }
    }
}

void dSndStateMgr_c::resetLpfAndFxSend() {
    resetBgmLpfAndFxSend();
    for (u32 id = dSndPlayerMgr_c::PLAYER_LINK_BODY; id <= dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, 0.0f, 15);
    }

    for (u32 id = dSndPlayerMgr_c::PLAYER_LINK_BODY; id <= dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setFxSend(id, 0.0f, 15);
    }
}

void dSndStateMgr_c::resetBgmLpfAndFxSend() {
    for (u32 id = dSndPlayerMgr_c::PLAYER_BGM; id < dSndPlayerMgr_c::PLAYER_BGM_BATTLE + 1; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, 0.0f, 15);
        dSndControlPlayerMgr_c::GetInstance()->setFxSend(id, 0.0f, 15);
    }
}

void dSndStateMgr_c::setBgmAndStageEffectLpf() {
    setBgmLpf();
    for (u32 id = dSndPlayerMgr_c::PLAYER_TG_SOUND; id < dSndPlayerMgr_c::PLAYER_AREA + 1; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, -0.55f, 15);
    }
}

void dSndStateMgr_c::setBgmLpf() {
    setBgmLpf(15);
}

void dSndStateMgr_c::setBgmLpf(s32 fadeFrames) {
    for (u32 id = dSndPlayerMgr_c::PLAYER_BGM; id < dSndPlayerMgr_c::PLAYER_BGM_BATTLE + 1; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, -0.55f, fadeFrames);
    }
}

void dSndStateMgr_c::resetBgmAndStageEffectLpf() {
    resetBgmLpf();
    for (u32 id = dSndPlayerMgr_c::PLAYER_TG_SOUND; id < dSndPlayerMgr_c::PLAYER_AREA + 1; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, 0.0f, 15);
    }
}

void dSndStateMgr_c::resetBgmLpf() {
    for (u32 id = dSndPlayerMgr_c::PLAYER_BGM; id < dSndPlayerMgr_c::PLAYER_BGM_BATTLE + 1; id++) {
        dSndControlPlayerMgr_c::GetInstance()->setLpfFreq(id, 0.0f, 15);
    }
}

void dSndStateMgr_c::calcFxSend3D() {
    if (mFxSend3D > mFxSend3DTarget) {
        mFxSend3D -= 0.02f;
        if (mFxSend3D < mFxSend3DTarget) {
            mFxSend3D = mFxSend3DTarget;
        }
    } else if (mFxSend3D < mFxSend3DTarget) {
        mFxSend3D += 0.02f;
        if (mFxSend3D > mFxSend3DTarget) {
            mFxSend3D = mFxSend3DTarget;
        }
    }
}

void dSndStateMgr_c::setFxSend3DTarget() {
    // TODO - weird control flow
    f32 target = mFxSend3DDefault;
    if (mFxSend3DNext > 0.0f) {
        target = mFxSend3DNext;
    } else if (mFxSend3DOverride > 0.0f) {
        target = mFxSend3DOverride;
    }

    if (target < 0.02f) {
        target = 0.02f;
    } else if (target > 0.3f) {
        target = 0.3f;
    }
    mFxSend3DTarget = target;
}

f32 dSndStateMgr_c::getUserParamVolume(u32 userParam) {
    s32 bit = 31;
    for (int i = 0; i < 4; i++) {
        if (((1 << bit) & userParam) != 0) {
            switch (bit) {
                case 31: return 0.3f;
                case 30: return 0.2f;
                case 29: return 0.1f;
                case 28: return 0.02f;
            }
        }

        bit--;
    }
    return 0.0f;
}

const char *dSndStateMgr_c::getStageName(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return "NULL";
    }
    return dSndStageInfo::sInfos[id].stageName;
}

const char *dSndStateMgr_c::getStageName4(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return "NULL";
    }
    return dSndStageInfo::sInfos[getSndStageId4(id)].stageName;
}

const char *dSndStateMgr_c::getCurrentStageName4() {
    return getStageName(dSndStageInfo::sInfos[mStageId].unk4);
}

s32 dSndStateMgr_c::getSndStageId(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1) {
                    return dSndStageInfo::sInfos[idx].unk1;
                }
                if (dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk1;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId3(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1 || dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk3;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId4(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1 || dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk4;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId2(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return SND_STAGE_MAX;
    }
    return dSndStageInfo::sInfos[id].unk2;
}

s32 dSndStateMgr_c::getSndStageId4(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return SND_STAGE_MAX;
    }
    return dSndStageInfo::sInfos[id].unk4;
}

s32 dSndStateMgr_c::getNextSndStageId(s32 id) {
    return getSndStageId(dScGame_c::nextSpawnInfo.getStageName(), dScGame_c::nextSpawnInfo.layer);
}

bool dSndStateMgr_c::specialLayerVersionExists(const char *stageName, s32 layer) {
    if (stageName != nullptr && layer > 0) {
        return getSndStageId(stageName, layer) != getSndStageId(stageName, 0);
    }

    return false;
}

u32 dSndStateMgr_c::getSoundIdForStageAndLayer(SoundIdLookup_e lookup, const char *stageName, s32 layer, s32 stageId) {
    // Ternary causes regswaps...
    const char *prefix = "BGM_";
    if (lookup == LOOKUP_SE_A) {
        prefix = "SE_A_";
    }
    SizedString<64> label;

    u32 soundId = -1;
    s32 round = 3;

    bool hasSpecialLayerVersion = false;
    if (layer > 0) {
        hasSpecialLayerVersion = specialLayerVersionExists(stageName, layer);
    }

    if (hasSpecialLayerVersion) {
        label.sprintf("%s%s_L%d", prefix, stageName, layer);
    } else if (layer > 0) {
        label.sprintf("%s%s_L%d", prefix, stageName, layer);
    } else {
        label.sprintf("%s%s", prefix, stageName);
        // We didn't bother with a _L layer variant, so we're going
        // in with the base variant
        round = 2;
    }

    while (round > 0) {
        if (lookup == LOOKUP_BGM_MAIN) {
            label += "_MAIN";
        }

        soundId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (soundId != -1) {
            break;
        }

        if (round == 3) {
            // If _L layer variant didn't exist, try the one without a layer...
            label.sprintf("%s%s", prefix, stageName);
        } else if (round == 2) {
            if (lookup == LOOKUP_BGM) {
                // If the one without a layer doesn't exist, continue with the one referenced
                // in field 3, unless it's the same as the original one
                s32 otherStageId = getSndStageId3(stageName, layer);
                if (stageId != otherStageId) {
                    label.sprintf("%s%s", prefix, getStageName(otherStageId));
                } else {
                    // break to prevent infinite loop
                    round = 0;
                }
            } else if (lookup == LOOKUP_SE_A) {
                // If the one without a layer doesn't exist, continue with the one referenced
                // in field 4, unless it's the same as the original one
                s32 otherStageId = getSndStageId4(stageName, layer);
                if (stageId != otherStageId) {
                    label.sprintf("%s%s", prefix, getStageName(otherStageId));
                } else {
                    // break to prevent infinite loop
                    break;
                }
            } else {
                break;
            }
        }

        round--;
    }

    return soundId;
}
