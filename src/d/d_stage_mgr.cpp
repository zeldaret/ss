#include "d/d_stage_mgr.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_bzs_types.h"
#include "d/d_heap.h"
#include "d/d_last.h"
#include "d/d_sc_game.h"
#include "d/d_stage_parse.h"
#include "d/d_sys.h"
#include "d/flag/flag_managers.h"
#include "egg/core/eggHeap.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_dvd.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/music_mgrs.h"
#include <cstddef>

SPECIAL_BASE_PROFILE(STAGE_MANAGER, dStageMgr_c, fProfile::STAGE_MANAGER, 0X5, 1536);

STATE_DEFINE(dStageMgr_c, ReadStageRes);
STATE_DEFINE(dStageMgr_c, ReadRoomRes);
STATE_DEFINE(dStageMgr_c, ReadObjectRes);
STATE_DEFINE(dStageMgr_c, ReadStageLayerRes);
STATE_DEFINE(dStageMgr_c, ReadLayerObjectRes);
STATE_DEFINE(dStageMgr_c, SoundLoadSceneData);
STATE_DEFINE(dStageMgr_c, CreateObject);
STATE_DEFINE(dStageMgr_c, ReadObjectSound);
STATE_DEFINE(dStageMgr_c, SceneChangeSave);
STATE_DEFINE(dStageMgr_c, RestartSceneWait);
STATE_DEFINE(dStageMgr_c, RestartScene);

dStageMgr_c::dStageMgr_c() : mStateMgr(*this, sStateID::null), mPhase(this, sCallbacks) {
    sInstance = this;
}

dStageMgr_c::~dStageMgr_c() {
    sInstance = nullptr;
}

void dStageMgr_c::initializeState_ReadStageRes() {
    CurrentStageArcManager::sInstance->setStage(dScGame_c::currentSpawnInfo.stageName);
}

void dStageMgr_c::executeState_ReadStageRes() {
    if (CurrentStageArcManager::sInstance->ensureAllEntriesLoaded() == 0) {
        mStateMgr.changeState(StateID_ReadRoomRes);
    }
}

void dStageMgr_c::finalizeState_ReadStageRes() {
    void *stageBzs = CurrentStageArcManager::sInstance->getData("dat/stage.bzs");
    if (stageBzs != nullptr) {
        parseStageBzs(-1, stageBzs);
        parseRoomStageBzs(-1, stageBzs);
    }
    fn_803618F0(ENEMY_SOUND_MGR);
}

void dStageMgr_c::initializeState_ReadRoomRes() {}

void dStageMgr_c::executeState_ReadRoomRes() {
    if (CurrentStageArcManager::sInstance->ensureAllEntriesLoaded() == 0 &&
        LayoutArcManager::sInstance->ensureAllEntriesLoaded() == 0) {
        mStateMgr.changeState(StateID_ReadObjectRes);
    }
}

void dStageMgr_c::finalizeState_ReadRoomRes() {
    if (rmpl != nullptr) {
        RMPL *itRmpl = rmpl;
        for (int i = 0; i < mRmplCount; itRmpl++, i++) {
            void *bzs = CurrentStageArcManager::sInstance->loadFromRoomArc(itRmpl->roomId, "dat/room.bzs");
            parseRoomStageBzs(itRmpl->roomId, bzs);
        }
    } else {
        u32 roomId = dScGame_c::currentSpawnInfo.room;
        void *bzs =
            CurrentStageArcManager::sInstance->loadFromRoomArc(dScGame_c::currentSpawnInfo.room, "dat/room.bzs");
        parseRoomStageBzs(roomId, bzs);
    }
}

void dStageMgr_c::initializeState_ReadObjectRes() {
    mStageObjCtrl.doLoad();
}

void dStageMgr_c::executeState_ReadObjectRes() {
    if (mStageObjCtrl.isLoaded()) {
        mStateMgr.changeState(StateID_ReadStageLayerRes);
    }
}

void dStageMgr_c::finalizeState_ReadObjectRes() {}

void dStageMgr_c::initializeState_ReadStageLayerRes() {
    CurrentStageArcManager::sInstance->loadFileFromExtraLayerArc(dScGame_c::currentSpawnInfo.layer);
}

void dStageMgr_c::executeState_ReadStageLayerRes() {
    if (CurrentStageArcManager::sInstance->ensureAllEntriesLoaded() == 0) {
        mStateMgr.changeState(StateID_ReadLayerObjectRes);
    }
}

void dStageMgr_c::finalizeState_ReadStageLayerRes() {}

const char demoName[] = "";
static const char *sSeekerStoneLayoutArcs[] = {
    "SeekerStone",
};

void dStageMgr_c::initializeState_ReadLayerObjectRes() {
    mDemoName = demoName;

    void *bzs = CurrentStageArcManager::sInstance->getData("dat/stage.bzs");
    if (bzs != nullptr) {
        parseBzsStageRoom(-1, bzs);
    }

    if (rmpl != nullptr) {
        RMPL *itRmpl = rmpl;
        for (int i = 0; i < mRmplCount; itRmpl++, i++) {
            void *bzs = CurrentStageArcManager::sInstance->loadFromRoomArc(itRmpl->roomId, "dat/room.bzs");
            parseBzsStageRoom(itRmpl->roomId, bzs);
        }
    } else {
        u32 roomId = dScGame_c::currentSpawnInfo.room;
        void *bzs =
            CurrentStageArcManager::sInstance->loadFromRoomArc(dScGame_c::currentSpawnInfo.room, "dat/room.bzs");
        parseBzsStageRoom(roomId, bzs);
    }

    if (dScGame_c::isStateLayerWithSeekerStoneHintMenu()) {
        mLayoutArcCtrl2.set(sSeekerStoneLayoutArcs, ARRAY_LENGTH(sSeekerStoneLayoutArcs));
        mLayoutArcCtrl2.load(dHeap::work2Heap.heap);
        addActorId(fProfile::LYT_SEEKER_STONE);
    }
    mLayerObjCtrl.doLoad();
}

void dStageMgr_c::executeState_ReadLayerObjectRes() {
    if (mLayerObjCtrl.isLoaded() && LayoutArcManager::sInstance->ensureAllEntriesLoaded() == 0) {
        mStateMgr.changeState(StateID_SoundLoadSceneData);
    }
}

extern "C" void fn_800C85D0(void *, EGG::Heap *heap, bool, void *jpc, void *jpn);
extern "C" void *FOR_LOADED_PARTICLES;

void dStageMgr_c::finalizeState_ReadLayerObjectRes() {
    if (mDemoName.len() != 0) {
        const char *name = mDemoName;
        void *jpc = OarcManager::sInstance->getSubEntryData(name, "dat/jparticle.jpc");
        if (jpc != nullptr) {
            void *jpn = OarcManager::sInstance->getSubEntryData(name, "dat/jparticle.jpn");
            fn_800C85D0(FOR_LOADED_PARTICLES, dHeap::work2Heap.heap, true, jpc, jpn);
        }
    }
}

static void *soundCallback(void *arg) {
    fn_80362730(ENEMY_SOUND_MGR);
    return reinterpret_cast<void *>(true);
}

void dStageMgr_c::initializeState_SoundLoadSceneData() {
    fn_803624F0(ENEMY_SOUND_MGR);
    mpDvdCallback = mDvd_callback_c::createOrDie(soundCallback, nullptr);
}

void dStageMgr_c::executeState_SoundLoadSceneData() {
    if (mpDvdCallback != nullptr && mpDvdCallback->mStatus != 0) {
        mStateMgr.changeState(StateID_CreateObject);
    }
}

void dStageMgr_c::finalizeState_SoundLoadSceneData() {
    mpDvdCallback->do_delete();
    mpDvdCallback = nullptr;
}

void dStageMgr_c::initializeState_CreateObject() {
    // too much egg
}

void dStageMgr_c::executeState_CreateObject() {
    if (!checkChildProcessCreateState()) {
        mStateMgr.changeState(StateID_ReadObjectSound);
    }
}

void dStageMgr_c::finalizeState_CreateObject() {}

static void *soundCallback2(void *arg) {
    fn_80365D20(ENEMY_SOUND_MGR);
    return reinterpret_cast<void *>(true);
}

void dStageMgr_c::initializeState_ReadObjectSound() {
    mpDvdCallback2 = mDvd_callback_c::createOrDie(soundCallback2, nullptr);
}

void dStageMgr_c::executeState_ReadObjectSound() {
    if (mpDvdCallback2 == nullptr) {
        return;
    }

    if (mpDvdCallback2->mStatus == 0) {
        return;
    }

    if (dScGame_c::sInstance != nullptr) {
        if (dScGame_c::sInstance->savePromptFlag() == true) {
            mStateMgr.changeState(StateID_SceneChangeSave);
        } else {
            mStateMgr.changeState(StateID_RestartSceneWait);
        }
        return;
    }

    mStateMgr.changeState(StateID_RestartSceneWait);
}

void dStageMgr_c::finalizeState_ReadObjectSound() {
    mpDvdCallback2->do_delete();
    // mpDvdCallback2 = nullptr;
    clearProcControlFlag(ROOT_DISABLE_EXECUTE);
    clearProcControlFlag(ROOT_DISABLE_DRAW);
    dLast_c::setExecuteCallback(lastExecuteCallback);
}

extern "C" void *LYT_SAVE_MGR;
extern "C" void fn_80285600(void*, int, int);
void dStageMgr_c::initializeState_SceneChangeSave() {
    dScGame_c::sInstance->setSavePromptFlag(false);
    if (LYT_SAVE_MGR != nullptr) {
        fn_80285600(LYT_SAVE_MGR, 3, 0);
    }
    dBase_c::s_NextExecuteControlFlags |= 1;
    dSys::setFrameRate(2);
}

void dStageMgr_c::executeState_SceneChangeSave() {
    if (LYT_SAVE_MGR != nullptr) {
        // "isNotSaving???"
        if (((u8*)LYT_SAVE_MGR)[0x119C] == true) {
            mStateMgr.changeState(StateID_RestartSceneWait);
        }
    } else {
        mStateMgr.changeState(StateID_RestartSceneWait);
    }
}

void dStageMgr_c::finalizeState_SceneChangeSave() {
    dBase_c::s_NextExecuteControlFlags &= ~1;
    dBase_c::s_DrawControlFlags &= ~1;
}

void dStageMgr_c::initializeState_RestartSceneWait() {
    dSys::setFrameRate(2);
}

void dStageMgr_c::executeState_RestartSceneWait() {
    if (mFader.isSettled()) {
        if (mFader.isStatus(mFaderBase_c::FADED_IN)) {
            if (field_0x88BC) {
                mStateMgr.changeState(StateID_RestartScene);
            }
        } else {
            mFader.fadeIn();
        }
        // TODO
        mFader.calc();
    }
}

void dStageMgr_c::finalizeState_RestartSceneWait() {}

void dStageMgr_c::initializeState_RestartScene() {
    triggerFade(dScGame_c::nextSpawnInfo.transitionType, dScGame_c::nextSpawnInfo.transitionFadeFrames);
    mFader.setFadeInType(dScGame_c::nextSpawnInfo.transitionType);
    fn_803625F0(ENEMY_SOUND_MGR, mFader.getFadeOutFrame());
}

void dStageMgr_c::executeState_RestartScene() {
    if (mFader.isStatus(mFaderBase_c::FADED_OUT)) {

    }
}

void dStageMgr_c::finalizeState_RestartScene() {
    commitAllFlagManagers();
    dSys::setFrameRate(1);
}
