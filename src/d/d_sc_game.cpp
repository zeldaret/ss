#include "d/d_sc_game.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_stage_mgr.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_fader_base.h"
#include "m/m_vec.h"
#include "s/s_FPhase.h"
#include "s/s_State.hpp"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/minigame_mgr.h"
#include "toBeSorted/music_mgrs.h"
#include "toBeSorted/some_gxf_thing.h"

#include <cstring>

SPECIAL_BASE_PROFILE(GAME, dScGame_c, fProfile::GAME, 0x4, 0x6F9);

STATE_VIRTUAL_DEFINE(dScGame_c, Stanby);
STATE_VIRTUAL_DEFINE(dScGame_c, Action);

SpawnInfo dScGame_c::currentSpawnInfo;
SpawnInfo dScGame_c::nextSpawnInfo(dScGame_c::currentSpawnInfo);

dScGame_c *dScGame_c::sInstance;
u32 dScGame_c::sUpdateFrameCount;
u8 dScGame_c::sCurrentLayer;
u8 dScGame_c::sReloaderType;
bool dScGame_c::sCopyFileBToCurrentAfterRespawn;
bool dScGame_c::sDoSomethingWithFileAOnTransition;
bool dScGame_c::sPreventClearingSomeFlag;

sFPhase<dScGame_c>::phaseCallback dScGame_c::sCallbacks[] = {
    &dScGame_c::cb1, &dScGame_c::cb2, &dScGame_c::cb3, nullptr
};

void dScGame_c::someGfxThingCallback() {
    dScGame_c *scGame = dScGame_c::GetInstance();
    scGame->somethingWithScreen();
    scGame->mFader.draw();
}

dScGame_c::dScGame_c()
    : mPhases(this, sCallbacks),
      mStateMgr(*this),
      mStageMgrRef(nullptr),
      mScreen1(0x85),
      mScreen2(0x89),
      field_0x228(0),
      mReloadTrigger(fProfile::PROFILE_MAX) {
    resetLinkState();
    mBeedleShopState = 0;
    mLastAreaType = 6;
    mType0PosFlag = 0;
    mSavePromptFlag = false;
    mPreventSaveRespawnInfo = true;
    sInstance = this;
    currentSpawnInfo.reset();
}

int dScGame_c::create() {
    sFPhaseBase::sFPhaseState phaseResult = mPhases.step();
    if (phaseResult != sFPhaseBase::PHASE_ALL_DONE) {
        return NOT_READY;
    }

    SomeGfxThing::setDrawCallback(someGfxThingCallback);
    return SUCCEEDED;
}

int dScGame_c::doDelete() {
    MinigameManager::destroy();
    mRelCtrl.do_unlink();
    mLayoutCtrl.release();

    return SUCCEEDED;
}

int dScGame_c::execute() {
    mFader.calc();
    mStateMgr.executeState();
    MinigameManager::execute();

    return SUCCEEDED;
}

int dScGame_c::draw() {
    mScreen1.addToDrawList();
    mScreen2.addToDrawList();

    return SUCCEEDED;
}

void dScGame_c::deleteReady() {
    SomeGfxThing::setDrawCallback(nullptr);
}

sFPhaseBase::sFPhaseState dScGame_c::cb1() {
    mFader.create();
    mFader.setFrames(15, 15);
    mFader.setTypes(1, 1);
    mFader.initForLyt();
    mRelCtrl.do_link();
    mLayoutCtrl.load(nullptr);

    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScGame_c::cb2() {
    if (!mRelCtrl.do_link()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    if (OarcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    if (LayoutArcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    MinigameManager::create();
    setReloadTrigger(fProfile::STAGE_MANAGER);
    mStateMgr.changeState(StateID_Stanby);
    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScGame_c::cb3() {
    if (checkChildProcessCreateState()) {
        return sFPhaseBase::PHASE_RETRY;
    }
    return sFPhaseBase::PHASE_NEXT;
}

void dScGame_c::initializeState_Stanby() {
    setProcControlFlag(ROOT_DISABLE_EXECUTE | ROOT_DISABLE_DRAW);
    if (mReloadTrigger == fProfile::STAGE_MANAGER) {
        copySpawnNextToCurrent();
    }
    dBase_c *base = dBase_c::createBase(mReloadTrigger, this, 0, OTHER);
    mStageMgrRef.link(base);
    mReloadTrigger = fProfile::PROFILE_MAX;
}

void dScGame_c::executeState_Stanby() {
    mStateMgr.changeState(StateID_Action);
}

void dScGame_c::finalizeState_Stanby() {}

void dScGame_c::initializeState_Action() {
    mFader.fadeIn();
}

void dScGame_c::executeState_Action() {
    if (mFader.isStatus(mFaderBase_c::FADED_OUT)) {
        if (attemptDelete()) {
            mStateMgr.changeState(StateID_Stanby);
        }
    } else if (mReloadTrigger != fProfile::PROFILE_MAX) {
        mFader.fadeOut();
        fn_80362150(ENEMY_SOUND_MGR, mFader.getFadeOutFrame());
    }
}

void dScGame_c::finalizeState_Action() {}

void dScGame_c::somethingWithScreen() {}

void dScGame_c::setRespawnInfo(const mVec3_c &linkPos, const mAng3_c &linkRot, bool unk) {
    bool isInThunderDragonTrial = MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) ||
                                          MinigameManager::isInMinigameState(MinigameManager::BOSS_RUSH) ?
                                      true :
                                      false;
    bool isInHordeFight = isCurrentStage("F403") && !StoryflagManager::sInstance->getFlag(134);
    s32 areaType = dStageMgr_c::GetInstance()->getAreaType();
    if (mPreventSaveRespawnInfo != true && !isInThunderDragonTrial && !isInHordeFight &&
        (areaType != 2 && !isCurrentStage("B100_1") && !sPreventClearingSomeFlag &&
         (areaType != 1 || (mLastAreaType != 1 && mLastAreaType != 2)))) {
        FileManager::GetInstance()->setT3Info(linkPos, linkRot);
        if (mLastAreaType != 5 && unk) {
            StoryflagManager::sInstance->unsetFlag(752);
        }
    }

    if (!isInThunderDragonTrial && isCurrentStage("B400") && mLastAreaType != 2) {
        FileManager::GetInstance()->copyCurrentToFileB();
    }
    sPreventClearingSomeFlag = 0;
    mPreventSaveRespawnInfo = false;
    mLastAreaType = areaType;
}

const LinkReloadInfo &dScGame_c::getLinkReloadInfo() const {
    return mReloadInfo;
}

bool dScGame_c::attemptDelete() {
    if (mStageMgrRef.p_owner == nullptr) {
        return true;
    } else {
        if (!mStageMgrRef.p_owner->delete_request) {
            mStageMgrRef.p_owner->deleteRequest();
        }
        return false;
    }
}

bool dScGame_c::setReloadTrigger(fProfile::PROFILE_NAME_e trigger) {
    if (mReloadTrigger != fProfile::PROFILE_MAX) {
        return false;
    }
    mReloadTrigger = trigger;
    return true;
}

void dScGame_c::setSpawnInfo(u8 roomid, const mVec3_c &pos, mAng rot) {
    mSpawnInfo.setSpawnData(currentSpawnInfo.stageName, roomid, sCurrentLayer, 0, pos, rot);
}

const SpawnInfoExt &dScGame_c::getSpawnInfo() const {
    return mSpawnInfo;
}

void dScGame_c::clearSpawnInfo() {
    mSpawnInfo.setSpawnData("", 0, 0, 0, mVec3_c::Zero, 0);
}

extern "C" void processEventFlags();
void dScGame_c::triggerExit(s32 room, u8 exitIndex, s32 forcedNight, s32 forcedTrial) {
    processEventFlags();
    // TODO
}

void SpawnInfoExt::setSpawnData(
    const char *i_stageName, u8 i_roomid, u8 i_layer, u8 i_entrance, const mVec3_c &pos, mAng rot
) {
    setData(
        i_stageName, i_roomid, i_layer, i_entrance, dScGame_c::currentSpawnInfo.night,
        dScGame_c::currentSpawnInfo.trial, 0, 15, -1
    );
    mType0Pos = pos;
    mType0RotY = rot;
}

void LinkReloadInfo::set(u8 roomid, const mVec3_c &pos, mAng rot, u32 linkParams) {
    mRoomId = roomid;
    mPosition = pos;
    mRotY = rot;
    mLinkParams = linkParams;
    mType0CsFlag = 1;
}

bool dScGame_c::isCurrentStage(const char *stage) {
    return dScGame_c::currentSpawnInfo.stageName == stage;
}

extern "C" void fn_801BB9C0();
extern "C" void *lbl_80575688;
extern "C" void fn_801909A0(void *);
extern "C" void maybeResetSkykeepPuzzle(bool);

void dScGame_c::copySpawnNextToCurrent() {
    if (sDoSomethingWithFileAOnTransition == true) {
        fn_801BB9C0();
        sDoSomethingWithFileAOnTransition = false;
    }
    if (std::strncmp(currentSpawnInfo.stageName, nextSpawnInfo.stageName, 2)) {
        fn_801909A0(lbl_80575688);
    }
    currentSpawnInfo = nextSpawnInfo;
    sCurrentLayer = currentSpawnInfo.layer;
    maybeResetSkykeepPuzzle(false);
}

void dScGame_c::actuallyTriggerEntrance(
    const char *stageName, u8 roomid, u8 layer, u8 entrance, int forcedNight, int forcedTrial, u8 transitionType,
    u16 transitionFadeFrames, s8 field0x28
) {
    s32 trial, night;
    if (forcedTrial == 2) {
        trial = currentSpawnInfo.trial;
    } else {
        trial = forcedTrial;
    }

    if (forcedNight == 2) {
        night = currentSpawnInfo.night;
    } else {
        night = forcedNight;
    }

    nextSpawnInfo.setData(
        stageName, roomid, layer, entrance, night, trial, transitionType, transitionFadeFrames, field0x28
    );

    if (dScGame_c::GetInstance() != nullptr) {
        dScGame_c::GetInstance()->setFadeInType(transitionType);
        dScGame_c::GetInstance()->setFadeOutType(transitionType);
        dScGame_c::GetInstance()->setFaderFrames(transitionFadeFrames);
        dScGame_c::GetInstance()->resetLinkState();
        dScGame_c::GetInstance()->setBeedleShopState(0);
    }
}

void SpawnInfo::setData(
    const char *i_stageName, u8 i_roomid, u8 i_layer, u8 i_entrance, s32 i_night, s32 i_trial, u8 i_transitionType,
    u16 i_fadeFrames, s8 i_field0x28
) {
    stageName = i_stageName;
    room = i_roomid;
    layer = i_layer;
    entrance = i_entrance;
    night = i_night;
    trial = i_trial;
    transitionType = i_transitionType;
    transitionFadeFrames = i_fadeFrames;
    unk = i_field0x28;
}
