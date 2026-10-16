#include "d/a/obj/d_a_obj_control_panel.h"
#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_D3_dummy.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_base.h"
#include "d/d_camera.h"
#include "d/d_cs_game.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_triangular.h"
#include "rvl/GX/GXTypes.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/misc_actor.h"


static const char* eventName = "ControlPanel";
static const char* firstAfterEvent = "AfterControlPanelFirst";
static const char* afterEvent = "AfterControlPanel";

static const u8 unused[] = { 0x00, 0xff, 0x00, 0xaa, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x43, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const void* getUnused() {
    return unused;
}

static const char* sSkyKeepRooms[] = {
    "D003_0",
    "D003_1",
    "D003_2",
    "D003_3",
    "D003_4",
    "D003_5",
    "D003_6",
    "D003_7",
};

const void* getUnused2() {
    return sSkyKeepRooms;
}

static InteractionTargetDef interactionTarget = {0,     3,     1,       EXAMINE_TALK,   0,    200.0f,
                                                           60.0f, 60.0f, -100.0f, 100.0f, 50.0f, 1.0f};



static mVec3_c sSpotPositions[9] = {
   mVec3_c( -43.2,
    14.78,
    -40.59),
    mVec3_c(0.0,
    14.78,
    -40.59),
    mVec3_c(43.2,
    14.78,
    -40.59),
    mVec3_c(-43.2,
    0.0,
    0.0),
    mVec3_c(0.0,
    0.0,
    0.0),
    mVec3_c(43.2,
    0.0,
    0.0),
    mVec3_c(-43.2,
    -14.78,
    40.59),
    mVec3_c(0.0,
    -14.78,
    40.59),
    mVec3_c(43.2,
    -14.78,
    40.59),
};


SPECIAL_ACTOR_PROFILE(OBJ_CONTROL_PANEL, dAcOCtrlPnl_c, fProfile::OBJ_CONTROL_PANEL, 0x159, 0, 3);

STATE_DEFINE(dAcOCtrlPnl_c, Wait);
STATE_DEFINE(dAcOCtrlPnl_c, Control);
STATE_DEFINE(dAcOCtrlPnl_c, EndEvent);

void enterMoveCb(void* actor) {
    if (actor == nullptr) return;
    static_cast<dAcOCtrlPnl_c*>(actor)->enterMove();
}

void finishMoveCb(void* actor) {
    if (actor == nullptr) return;
    static_cast<dAcOCtrlPnl_c*>(actor)->finishMove();
}

void afterControlPanelFirstCb(void* actor) {
    if (actor == nullptr) return;
    static_cast<dAcOCtrlPnl_c*>(actor)->afterControlPanelFirst();
}

void afterControlPanelCb(void* actor) {
    if (actor == nullptr) return;
    static_cast<dAcOCtrlPnl_c*>(actor)->afterControlPanel();
}

bool dAcOCtrlPnl_c::createHeap() {
    const char* oarcName = "ControlPanel";
    mRes = nw4r::g3d::ResFile(getOarcResFile(oarcName));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("ControlPanel");
    if (!mModel.create(mdl, &mAllocator, 0x120, 1, nullptr)) {
        return false;
    }
    void* dzb = getOarcDZB(oarcName, "ControlPanel");
    void* plc = getOarcPLC(oarcName, "ControlPanel");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 0x81, &mWorldMtx, &mScale);
}

int dAcOCtrlPnl_c::actorCreate() {
    CREATE_ALLOCATOR_SIZE(dAcOCtrlPnl_c, 0x8000);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcceleration = 0.f;
    mMaxSpeed = 0.f;
    mBoundingBox.Set(mVec3_c(-200.f, -0.f, -200.f), mVec3_c(200.f, 200.f, 200.f));
    updatePuzzleFromFile();
    updateTilePositions();
    mMovingTile = -1;
    setupNeighbors();
    updateDoorSceneflags();
    if ((s32)getFromParams(0x18, 3) == 1) {
        return FAILED;
    }
    char* plateModels[] = {
        "CPPlate1",
        "CPPlate2",
        "CPPlate3",
        "CPPlate4",
        "CPPlate5",
        "CPPlate6",
        "CPPlate7",
        "CPPlate8",
    };
    nw4r::g3d::ResMdl resMdl = mModel.getResMdl();
    for (int i = 0; i < 8; i++) {
        nw4r::g3d::ResNode node = resMdl.GetResNode(plateModels[i]);
        mCallback.setNodeID(i, node.GetID());
        mCallback.setPosAndScale(i, mTilePositions[i], 1.f);
    }
    mModel.setLocalMtx(mWorldMtx);
    mModel.setCallback(&mCallback);
    mModel.calc(false);
    mCurrentlyHoveredTile = -1;
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dAcOCtrlPnl_c::actorPostCreate() {
    dAcOD3Dummy_c* ptr = static_cast<dAcOD3Dummy_c*>(fManager_c::searchBaseByProfName(fProfile::OBJ_D3_DUMMY));
    mD3DummyRef.link(ptr);
    static const char* plateModels[] = {
        "m_CPPlate1",
        "m_CPPlate2",
        "m_CPPlate3",
        "m_CPPlate4",
        "m_CPPlate5",
        "m_CPPlate6",
        "m_CPPlate7",
        "m_CPPlate8",
    };
    for (int i = 0; i < 8; i++) {
        mTilePlateMatIDs[i] = mModel.getMatID(plateModels[i]);
        if (mTilePlateMatIDs[i] < 0) {
            return FAILED;
        }
    }
    // just a guess
    static const char* triforcePlates[] = {
        "m_CPPlate1",
        "m_CPPlate3",
        "m_CPPlate4",
    };
    for (int i = 0; i < 3; i++) {
        mTriforceMatIDs[i] = mModel.getMatID(triforcePlates[i]);
        if (mTriforceMatIDs[i] < 0) {
            return FAILED;
        }
    }
    if (!mModel.getResMdl().GetResMat("m_CPPlate1").GetResMatTevColor().GXGetTevColor(GX_TEVREG1, &mNormalPlateColor)) {
        return FAILED;
    }
    return SUCCEEDED;
}

int dAcOCtrlPnl_c::doDelete() {
    sIsInSkykeepPuzzle = false;
    return SUCCEEDED;
}

int dAcOCtrlPnl_c::actorExecute() {
    mTimer++;
    mStateMgr.executeState();

    for (int i = 0; i < 8; i++) {
        mCallback.setPosAndScale(i, mTilePositions[i], mTileScales[i]);
    }
    updateTileColors();
    updateTileScales();
    mModel.calc(false);
    return SUCCEEDED;
}

int dAcOCtrlPnl_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOCtrlPnl_c::initializeState_Wait() {}
void dAcOCtrlPnl_c::executeState_Wait() {
    AttentionManager::GetInstance()->addTarget(*this, interactionTarget, 0, nullptr);
    if (EventManager::isInEvent(this, eventName)) {
        handleEvent();
        mStateMgr.changeState(StateID_Control);
    }
}
void dAcOCtrlPnl_c::finalizeState_Wait() {}
void dAcOCtrlPnl_c::initializeState_Control() {}
void dAcOCtrlPnl_c::executeState_Control() {
    handleEvent();
    dCamera_c* camera = dScGame_c::getCamera();
    if (camera != nullptr) {
        f32 camVal = 60.f;
        mVec3_c local48(0.f, 240.f, 0.f);
        mVec3_c local54(0.f, 0.f, 0.f);
        f32 fVar1 = nw4r::math::CosDeg(20);
        f32 fVar2 = nw4r::math::SinDeg(20);
        local54.y += (fVar2 * 100.f) / fVar1 + 70.f;
        local48.rotX(0xe39);
        fVar1 = nw4r::math::CosDeg(20);
        fVar2 = nw4r::math::SinDeg(20);
        local48.y += (fVar2 * 100.f) / fVar1 + 70.f;
        local48.rotY(mRotation.y);
        local54.rotY(mRotation.y);
        local48 += mPosition;
        local54 += mPosition;
        camera->setEventCamView(local54, local48, camVal, 0.f);
    }
}
void dAcOCtrlPnl_c::finalizeState_Control() {}
void dAcOCtrlPnl_c::initializeState_EndEvent() {
    handleEvent();
}
void dAcOCtrlPnl_c::executeState_EndEvent() {
    handleEvent();
}
void dAcOCtrlPnl_c::finalizeState_EndEvent() {
    SceneflagManager::sInstance->setFlag(mRoomID, getFromParams(0x10, 0xFF));
}

void dAcOCtrlPnl_c::doInteraction(s32) {
    Event event(eventName, getOarcZev("ControlPanel"), 100, 0x100001, (void*)enterMoveCb, (void*)finishMoveCb);
    mEvent.scheduleEvent(event, 0);
}

void dAcOCtrlPnl_c::enterMove() {
    handleLinkObjProp0x200(false);
    mCurrentlyHoveredSpot = 0xFF;
    mCurrentlyHoveredTile = -1;
    mHasMovedATile = false;
    mLeavePuzzle = false;
    updatePuzzleFromFile();
    sIsInSkykeepPuzzle = true;
}
void dAcOCtrlPnl_c::finishMove() {
    handleLinkObjProp0x200(true);
    bool changed = changedPuzzle();
    sIsInSkykeepPuzzle = false;
    savePuzzleState();
    if (mD3DummyRef.isLinked()) {
        mD3DummyRef.get()->setField_0x1570(1);
    }
    if (changed) {
        mStateMgr.changeState(StateID_EndEvent);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOCtrlPnl_c::afterControlPanelFirst() {
    mStateMgr.changeState(StateID_Wait);
}
void dAcOCtrlPnl_c::afterControlPanel() {
    mStateMgr.changeState(StateID_Wait);
}
// returns true when the tile finished moving
bool dAcOCtrlPnl_c::handleSlidingTile() {
    if (doChaseTilePosToTarget()) {
        mMovingTile = -1;
        return true;
    } else {
        return false;
    }
}
void dAcOCtrlPnl_c::updateTileScales() {
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    for (int i = 0; i < 8; i++) {
        if (i == mCurrentlyHoveredTile && hasAdjacentEmptySpot(mCurrentlyHoveredTile)) {
            mTileScales[i] = 1.1f;
        } else {
            mTileScales[i] = 1.0f;
        }
    }
}
bool dAcOCtrlPnl_c::hasAdjacentEmptySpot(s32 idx) {
    s32 out = -1;
    return getAdjacentEmptySpot(idx, &out);
}

bool dAcOCtrlPnl_c::getAdjacentEmptySpot(s32 tileID, s32* pEmptySpot) {
    if (tileID < 0) {
        return false;
    }
    if (tileID == getCurrentSkyKeepRoomTileId()) {
        return false;
    }
    s32 tileSpot = -1;
    for (s32 spot = 0; spot < 9; spot++) {
        if (tileID == mSpots[spot].mCurrentTile) {
            tileSpot = spot;
            break;
        }
    }
    if (tileSpot < 0) {
        return false;
    }
    for (int direction = 0; direction < 4; direction++) {
        dAcOCtrlPnl_c::Spot* adjacent = mSpots[tileSpot].getNeighbor(direction);
        if (adjacent != nullptr && adjacent->mCurrentTile == 0xFF) {
            *pEmptySpot = adjacent->mSpot;
            return true;
        }
    }
    return false;
}
void dAcOCtrlPnl_c::setupNeighbors() {
    mSpots[0].mNeighbors[0] = nullptr;
    mSpots[0].mNeighbors[1] = &mSpots[1];
    mSpots[0].mNeighbors[2] = &mSpots[3];
    mSpots[0].mNeighbors[3] = nullptr;
    mSpots[1].mNeighbors[0] = nullptr;
    mSpots[1].mNeighbors[1] = &mSpots[2];
    mSpots[1].mNeighbors[2] = &mSpots[4];
    mSpots[1].mNeighbors[3] = &mSpots[0];
    mSpots[2].mNeighbors[0] = nullptr;
    mSpots[2].mNeighbors[1] = nullptr;
    mSpots[2].mNeighbors[2] = &mSpots[5];
    mSpots[2].mNeighbors[3] = &mSpots[1];
    mSpots[3].mNeighbors[0] = &mSpots[0];
    mSpots[3].mNeighbors[1] = &mSpots[4];
    mSpots[3].mNeighbors[2] = &mSpots[6];
    mSpots[3].mNeighbors[3] = nullptr;
    mSpots[4].mNeighbors[0] = &mSpots[1];
    mSpots[4].mNeighbors[1] = &mSpots[5];
    mSpots[4].mNeighbors[2] = &mSpots[7];
    mSpots[4].mNeighbors[3] = &mSpots[3];
    mSpots[5].mNeighbors[0] = &mSpots[2];
    mSpots[5].mNeighbors[1] = nullptr;
    mSpots[5].mNeighbors[2] = &mSpots[8];
    mSpots[5].mNeighbors[3] = &mSpots[4];
    mSpots[6].mNeighbors[0] = &mSpots[3];
    mSpots[6].mNeighbors[1] = &mSpots[7];
    mSpots[6].mNeighbors[2] = nullptr;
    mSpots[6].mNeighbors[3] = nullptr;
    mSpots[7].mNeighbors[0] = &mSpots[4];
    mSpots[7].mNeighbors[1] = &mSpots[8];
    mSpots[7].mNeighbors[2] = nullptr;
    mSpots[7].mNeighbors[3] = &mSpots[6];
    mSpots[8].mNeighbors[0] = &mSpots[5];
    mSpots[8].mNeighbors[1] = nullptr;
    mSpots[8].mNeighbors[2] = nullptr;
    mSpots[8].mNeighbors[3] = &mSpots[7];
    for (int i = 0; i < 9; i++) {
        mSpots[i].mSpot = i;
    }
    updateSpotTilesFromPuzzle();
}

void dAcOCtrlPnl_c::handleEvent() {
    u16 flow = getFromParams(0, 0xFFFF);
    switch (mEvent.getCurrentEventCommand()) {
        case 'wait':
            mEvent.advanceNext();
            break;
        case 'ctrl':
            handleControl();
            if (mLeavePuzzle) {
                handleLinkObjProp0x200(true);
                mCurrentlyHoveredSpot = 0xFF;
                mCurrentlyHoveredTile = -1;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CtrlPnl_DECIDE);
                mEvent.advanceNext();
            }
            break;
        case 'msg ':
            if (mEvent.isAdvance()) {
                s32 part2 = flow < 10000U ? 1000 : 100;
                s32 truncate = (flow / part2);

                u16 lower = flow - truncate * part2;

                s32 part1 = flow < 10000U ? 1000 : 100;
                u16 upper = flow / part1;
                mFlowMgr.triggerEntryPoint(upper, lower, 0, 0);
                dLytMsgWindow_c::setFlowMgr(&mFlowMgr);
            } else {
                mFlowMgr.checkFinished();
                if (mFlowMgr.fn_800C40F0()) {
                    mEvent.advanceNext();
                }
            }
            break;
        case 'end ':
            void *zev = getOarcZev("ControlPanel");
            if (changedPuzzle()) {
                if (isFirstTime()) {
                    Event event(firstAfterEvent, zev, 100, 0x100001, nullptr, (void*)afterControlPanelFirstCb);
                    EventManager::setEvent(this, &event, nullptr);
                } else {
                    Event event(afterEvent, zev, 100, 0x100001, nullptr, (void*)afterControlPanelCb);
                    EventManager::setEvent(this, &event, nullptr);
                }
            }
            mEvent.advanceNext();
            break;
    }
}

// non matching
void dAcOCtrlPnl_c::handleControl() {
    bool canGrabTile = hasAdjacentEmptySpot(mCurrentlyHoveredTile);
    if (mIsGrabbing) {
        dLytDobutton_c::setActionTextStuff(dLytDobutton_c::ICON_29, dLytDobutton_c::ACT_DO_MOVE_2, true);
    } else if (canGrabTile) {
        dLytDobutton_c::setActionTextStuff(dLytDobutton_c::ICON_11, dLytDobutton_c::ACT_DO_GRAB_3, true);
    } else {
        dLytDobutton_c::setActionTextStuff(dLytDobutton_c::ICON_4, dLytDobutton_c::ACT_DO_SELECT, true);
    }
    if (!mIsGrabbing) {
        if (canGrabTile) {
            dLytMeter_c::GetInstance()->setFlags(0x200);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_INFO_GRAB_2);
        } else {
            dLytMeter_c::GetInstance()->clearFlags(0x200);
        }
    }
    dLytMeter_c::GetInstance()->setFlags(0x10);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::ACT_IE_ETC_DONE);
    dLytMeter_c::GetInstance()->setFlags(0x400);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_ETC_RESET_PANELS);
    dLytMeter_c::GetInstance()->clearFlags(0x1);
    dLytMeter_c::GetInstance()->clearFlags(0x4);
    dLytMeter_c::GetInstance()->clearFlags(0x8);
    dLytMeter_c::GetInstance()->clearFlags(0x40);
    dLytMeter_c::GetInstance()->clearFlags(0x80);
    dLytMeter_c::GetInstance()->clearFlags(0x800);
    dLytMeter_c::GetInstance()->setFlags(0x2);
    LytDoButtonRelated::setCrossDown(LytDoButtonRelated::ACT_IE_ETC_CENTER, true);
    bool downPressed = dPad::getDownTrigDown();
    if (downPressed) {
        dPad::ex_c::m_current_ex->centerCursor(mPad::getCurrentCoreID(), true);
    }
    dCsGame_c* game = dCsGame_c::GetInstance();
    if (game != nullptr) {
        if (game != nullptr) {
            if (mIsGrabbing) {
                game->pointerCatSet();
            } else {
                game->pointerDefSet();
            }
        }
        mVec2_c cursorPos = game->getCursorIf()->getCursorPos();
        // non matching: weird branches here
        bool allowInteraction = true;
        if (mMovingTile >= 0) {
            if (handleSlidingTile()) {
                mHasMovedATile = true;
                mIsGrabbing = false;
                u8 tile0 = mSpots[mCurrentlyHoveredSpot].mCurrentTile;
                u8 tile1 = mSpots[mGrabTargetSpot].mCurrentTile;
                mSpots[mCurrentlyHoveredSpot].mCurrentTile = tile1;
                mSpots[mGrabTargetSpot].mCurrentTile = tile0;
                setTileID(mGrabTargetSpot, tile0);
                setTileID(mCurrentlyHoveredSpot, tile1);
                updateDoorSceneflags();
                dAcOD3Dummy_c* dummy = mD3DummyRef.get();
                if (dummy != nullptr) {
                    savePuzzleState();
                    dummy->setField_0x1570(1);
                }
            }
            allowInteraction = false;
        }
        if (!mIsGrabbing) {
            if (dPad::getDownTrigB()) {
                mLeavePuzzle = true;
                allowInteraction = false;
            } else if (dPad::getDownTrigC()) {
                if (changedPuzzle()) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CtrlPnl_RESET);
                }
                resetPuzzleState();
                updateTilePositions();
                updateSpotTilesFromPuzzle();
                updateDoorSceneflags();
                allowInteraction = false;
            }
        }
        if (allowInteraction) {
            if (!mIsGrabbing) {
                s32 hoveredSpot = getSpotForCursor(cursorPos);
                mCurrentlyHoveredSpot = hoveredSpot;
                if (hoveredSpot == 0xFF) {
                    mCurrentlyHoveredTile = -1;
                } else {
                    s32 oldHoveredTile = mCurrentlyHoveredTile;
                    s32 hoveredTile = getTileID(hoveredSpot);
                    mCurrentlyHoveredTile = hoveredTile;
                    if (hoveredTile == 0xFF) {
                        mCurrentlyHoveredTile = -1;
                    }
                    if (oldHoveredTile != mCurrentlyHoveredTile && hasAdjacentEmptySpot(mCurrentlyHoveredTile)) {
                        startSound(SE_CtrlPnl_TAKE);
                    }
                }
                s32 emptyAdjacentSpot = -1;
                // check if the player grabbed the tile
                if (mCurrentlyHoveredTile >= 0 && dPad::getDownTrigA() && getAdjacentEmptySpot(mCurrentlyHoveredTile, &emptyAdjacentSpot)) {
                    mIsGrabbing = true;
                    mGrabTargetMoveDirection = getMoveDirection(mCurrentlyHoveredSpot, emptyAdjacentSpot);
                    mGrabStartCursorPos = cursorPos;
                    mGrabTargetSpot = emptyAdjacentSpot;
                    mGrabTargetPosition = sSpotPositions[emptyAdjacentSpot];
                }
            } else if(dPad::getUpTrigA()) {
                mIsGrabbing = false;
            } else {
                mVec2_c diff = cursorPos - mGrabStartCursorPos;
                if (mGrabTargetMoveDirection == 0) {
                    if (diff.y > 30.f) {
                        mMovingTile = mCurrentlyHoveredTile;
                        startSound(SE_CtrlPnl_SLIDE);
                    }
                } else if (mGrabTargetMoveDirection == 1) {
                    if (diff.x > 30.f) {
                        mMovingTile = mCurrentlyHoveredTile;
                        startSound(SE_CtrlPnl_SLIDE);
                    }
                } else if (mGrabTargetMoveDirection == 2) {
                    if (diff.y < -30.f) {
                        mMovingTile = mCurrentlyHoveredTile;
                        startSound(SE_CtrlPnl_SLIDE);
                    }
                } else {
                    if (diff.x < -30.f) {
                        mMovingTile = mCurrentlyHoveredTile;
                        startSound(SE_CtrlPnl_SLIDE);
                    }
                }
            }
        }
    }
}

void dAcOCtrlPnl_c::updateDoorSceneflags() {
    for (int spot = 0; spot < 9; spot++) {
        s32 tile = mSpots[spot].mCurrentTile;
        if (tile != 0xFF) {
            for (int direction = 0; direction < 4; direction++) {
                if (checkSKDoorHasSceneflag(tile, direction)) {
                    s32 sceneflag = getSKDoorSceneflag(tile, direction);
                    if (spot == 7 && direction == 2 && checkSKDoorHasSceneflag(tile, direction)) {
                        setSceneflag(sceneflag);
                    } else {
                        dAcOCtrlPnl_c::Spot* neighbor = mSpots[spot].mNeighbors[direction];
                        if (neighbor == nullptr) {
                            unsetSceneflag(sceneflag);
                        } else {
                            s32 curTile = neighbor->mCurrentTile;
                            s32 otherDirection;
                            if (direction == 0) {
                                otherDirection = 2;
                            }
                            if (direction == 2) {
                                otherDirection = 0;
                            }
                            if (direction == 3) {
                                otherDirection = 1;
                            }
                            if (direction == 1) {
                                otherDirection = 3;
                            }
                            if (curTile == 0xFF) {
                                unsetSceneflag(sceneflag);
                            } else if (checkSKDoorHasSceneflag(curTile, otherDirection)) {
                                setSceneflag(sceneflag);
                            } else {
                                unsetSceneflag(sceneflag);
                            }
                        }
                    }
                }
            }
        }
    }
}

inline bool checkSpotBounds(f32* spotBounds, const mVec2_c& vec, mVec2_c& cursorPos) {
    return vec.x + spotBounds[0] < cursorPos.x &&
            cursorPos.x < vec.x + spotBounds[2] &&
            vec.y + spotBounds[1] < cursorPos.y &&
            cursorPos.y < vec.y + spotBounds[3];
}

u8 dAcOCtrlPnl_c::getSpotForCursor(mVec2_c& cursorPos) {
    f32 spotBoundaries[9][4];
    mVec3_c vec;
    spotBoundaries[0][0] = -90.0;
    spotBoundaries[0][1] = 40.0;
    spotBoundaries[0][2] = -30.0;
    spotBoundaries[0][3] = 120.0;
    spotBoundaries[1][0] = -30.0;
    spotBoundaries[1][1] = 40.0;
    spotBoundaries[1][2] = 30.0;
    spotBoundaries[1][3] = 120.0;
    spotBoundaries[2][0] = 30.0;
    spotBoundaries[2][1] = 40.0;
    spotBoundaries[2][2] = 90.0;
    spotBoundaries[2][3] = 120.0;
    spotBoundaries[3][0] = -90.0;
    spotBoundaries[3][1] = -40.0;
    spotBoundaries[3][2] = -30.0;
    spotBoundaries[3][3] = 40.0;
    spotBoundaries[4][0] = -30.0;
    spotBoundaries[4][1] = -40.0;
    spotBoundaries[4][2] = 30.0;
    spotBoundaries[4][3] = 40.0;
    spotBoundaries[5][0] = 30.0;
    spotBoundaries[5][1] = -40.0;
    spotBoundaries[5][2] = 90.0;
    spotBoundaries[5][3] = 40.0;
    spotBoundaries[6][0] = -90.0;
    spotBoundaries[6][1] = -120.0;
    spotBoundaries[6][2] = -30.0;
    spotBoundaries[6][3] = -40.0;
    spotBoundaries[7][0] = -30.0;
    spotBoundaries[7][1] = -120.0;
    spotBoundaries[7][2] = 30.0;
    spotBoundaries[7][3] = -40.0;
    spotBoundaries[8][0] = 30.0;
    spotBoundaries[8][1] = -120.0;
    spotBoundaries[8][2] = 90.0;
    spotBoundaries[8][3] = -40.0;
    getScreenCenter(vec);
    f32 (*current)[4] = spotBoundaries;
    s32 j = 0;
    for (s32 remaining = 9; remaining != 0; remaining--, j++) {
        current = spotBoundaries;
        current += j;
        if (vec.x + (*current)[0] < cursorPos.x &&
            cursorPos.x < vec.x + (*current)[2] &&
            vec.y + (*current)[1] < cursorPos.y &&
            cursorPos.y < vec.y + (*current)[3]
        ) {
            return j;
        }
    }
    return 0xFF;
}

// non matching
u8 dAcOCtrlPnl_c::getMoveDirection(s32 currentSpot, s32 targetSpot) {
    s32 x1 = currentSpot % 3;
    s32 x2 = targetSpot % 3;
    s32 y1 = currentSpot / 3;
    s32 y2 = targetSpot / 3;
    if (x1 != x2 && y1 != y2) {
        return 0xFF;
    }
    // non matching: regswap
    s32 xAbs = EGG::Math<int>::abs(x1 - x2);
    s32 yAbs = EGG::Math<int>::abs(y1 - y2);
    if (xAbs > 1 || yAbs > 1) {
        return 0xFF;
    }
    s32 xDiff = x2 - x1;
    s32 yDiff = y2 - y1;
    if (xDiff == 1) {
        return 1;
    }
    if (xDiff == -1) {
        return 3;
    }
    if (yDiff == 1) {
        return 2;
    }
    if (yDiff == -1) {
        return 0;
    }
    return 0xFF;
}

void dAcOCtrlPnl_c::updatePuzzleFromFile() {
    for (int i = 0; i < 9; i++) {
        mSkykeepPuzzle[i] = FileManager::GetInstance()->getSkykeepPuzzleTile(i);
    }
    for (int i = 0; i < 9; i++) {
        mSkykeepPuzzleOriginal[i] = mSkykeepPuzzle[i];
    }
}

void dAcOCtrlPnl_c::setTileID(u32 slot, u32 tile) {
    if (slot > 8) {
        return;
    }
    if (tile != 0xFF && tile > 7) {
        return;
    }
    mSkykeepPuzzle[slot] = tile;
}

u8 dAcOCtrlPnl_c::getTileID(u32 spot) {
    if (spot <= 8) {
        return mSkykeepPuzzle[spot];
    }
    return 0xFF;
}

void dAcOCtrlPnl_c::resetPuzzleState() {
    for (int i = 0; i < 9; i++) {
        mSkykeepPuzzle[i] = mSkykeepPuzzleOriginal[i];
    }
}

void dAcOCtrlPnl_c::savePuzzleState() {
    for (int i = 0; i < 9; i++) {
        FileManager::GetInstance()->setSkykeepPuzzle(i, mSkykeepPuzzle[i]);
    }
}

void dAcOCtrlPnl_c::updateTilePositions() {
    for (int i = 0; i < 9; i++) {
        s32 tile = mSkykeepPuzzle[i];
        if (tile != 0xFF) {
            mTilePositions[tile] = sSpotPositions[i];
        }
    }
}

void dAcOCtrlPnl_c::updateSpotTilesFromPuzzle() {
    for (int i = 0; i < 9; i++) {
        mSpots[i].mCurrentTile = mSkykeepPuzzle[i];
    }
}

// non matching
bool dAcOCtrlPnl_c::isFirstTime() {
    u8 flag = getFromParams(0x10, 0xFF);
    // non matching: first comparison is weird
    return flag >= 0xFF || (flag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag));
}

bool dAcOCtrlPnl_c::changedPuzzle() {
    for (int i = 0; i < 9; i++) {
        if (mSkykeepPuzzleOriginal[i] != getTileID(i)) {
            return true;
        }
    }
    return false;
}

void dAcOCtrlPnl_c::getScreenCenter(mVec3_c& outVec) {
    mVec3_c local = mPosition + sSpotPositions[4];
    s32 _weird_zero = 0;
    local.y += (146.f + _weird_zero);
    d3d::worldToScreen(outVec, local);
}

void dAcOCtrlPnl_c::handleLinkObjProp0x200(bool shouldUnset) {
    if (dAcPy_c::GetLinkM() == nullptr) {
        return;
    }
    if (shouldUnset) {
        dAcPy_c::GetLinkM()->unsetObjectProperty(OBJ_PROP_0x200);
    } else {
        dAcPy_c::GetLinkM()->setObjectProperty(OBJ_PROP_0x200);
    }
}

void dAcOCtrlPnl_c::callback_c::timingB(u32 nodeID, nw4r::g3d::WorldMtxManip * result, nw4r::g3d::ResMdl) {
    bool isTileNode = false;
    for (int i = 0; i < 8; i++) {
        if (mNodeIDs[i] == nodeID) {
            isTileNode = true;
            break;
        }
    }
    if (isTileNode) {
        int tileId = -1;
        for (int i = 0; i < 8; i++) {
            if (mNodeIDs[i] == nodeID) {
                tileId = i;
            }
        }
        mMtx_c mtx;
        result->GetMtx(mtx);
        mtx.transM(field_0x0[tileId]);
        mtx.scaleM(field_0x20[tileId],field_0x20[tileId],field_0x20[tileId]);
        result->SetMtx(mtx);
    }
}

void dAcOCtrlPnl_c::updateTileColors() {
    ITEM_ID triforceItemflags[3] = {
        ITEM_TRI_COURAGE,
        ITEM_TRI_POWER,
        ITEM_TRI_WISDOM,
    };
    mColor triforceColors[3] = {
        mColor(0, 0xFF, 0, 0),
        mColor(0xFF, 0, 0, 0),
        mColor(0, 0, 0xFF, 0),
    };
    mColor def;
    for (int i = 0; i < 3; i++) {
        if (ItemflagManager::sInstance->getFlagDirect(triforceItemflags[i])) {
            mModel.setTevColor(mTriforceMatIDs[i], GX_TEVREG2, mColor(0,0,0,0), false);
        } else {
            def.r = triforceColors[i].r * (nw4r::math::SinIdx(mTimer * 0xFFFF / 0x3C) + 1.f) * 0.5f;
            def.g = triforceColors[i].g * (nw4r::math::SinIdx(mTimer * 0xFFFF / 0x3C) + 1.f) * 0.5f;
            def.b = triforceColors[i].b * (nw4r::math::SinIdx(mTimer * 0xFFFF / 0x3C) + 1.f) * 0.5f;
            def.a = 0;
            mModel.setTevColor(mTriforceMatIDs[i], GX_TEVREG2, def, false);
        }
    }
    for (int i = 0; i < 8; i++) {
        if (i == getCurrentSkyKeepRoomTileId()) {
            mModel.setTevColor(mTilePlateMatIDs[i], GX_TEVREG1, mColor(0,0,0,0), false);
        } else {
            mModel.setTevColor(mTilePlateMatIDs[i], GX_TEVREG1, mNormalPlateColor, false);
        }
    }
}

// missing 0x0000003C in rodata
