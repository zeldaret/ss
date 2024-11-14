#include "d/a/obj/d_a_obj_underground_switch.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resanmclr.h"
#include "nw4r/g3d/g3d_resmdl.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_UG_SWITCH, dAcOUgSwitch_c, fProfile::OBJ_UG_SWITCH, 0x15A, 0, 3);

STATE_DEFINE(dAcOUgSwitch_c, Off);
STATE_DEFINE(dAcOUgSwitch_c, Active);
STATE_DEFINE(dAcOUgSwitch_c, On);

const f32 dAcOUgSwitch_c::sActivationRadius = 40.0f;
const u8 dAcOUgSwitch_c::sEventTimer = 15;

bool dAcOUgSwitch_c::createHeap() {
    // Why. Regswap...
    void *data = getOarcResFile("SwitchPass");
    
    mRes = data;
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("SwitchPass");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120));
    
    mRes = data;
    mdl = mRes.GetResMdl("SwitchPass");
    nw4r::g3d::ResAnmClr clr = mRes.GetResAnmClr("SwitchPass_Light");
    TRY_CREATE(mAnmClr.create(mdl, clr, &heap_allocator, nullptr, 1));
    
    mMdl.setAnm(mAnmClr);

    return true;
}

int dAcOUgSwitch_c::create() {
    mActiveSceneFlag = params & 0xFF;
    field_0x3BD = (params >> 8) & 0xFF;
    setVariant((params >> 16) & 0xF);
    if (!initAllocatorWork1Heap(0x1000, "dAcOUgSwitch_c::m_allocator", 0x20)) {
        return FAILED;
    }

    mOnEventTimer = 0;
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setPriorityDraw(0x82, 0x7F);
    if (!SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
        if (mVariant == 0) {
            mStateMgr.changeState(StateID_Active);
        } else {
            mStateMgr.changeState(StateID_Off);
        }
    } else {
        mStateMgr.changeState(StateID_On);
    }
    boundingBox.Set(mVec3_c(-50.0f, -20.0f, -50.0f), mVec3_c(50.0f, 20.0f, 50.0f));

    return SUCCEEDED;
}

int dAcOUgSwitch_c::doDelete() {
    return SUCCEEDED;
}

int dAcOUgSwitch_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOUgSwitch_c::actorExecuteInEvent() {
    if (EventManager::isInEvent(this, "OnSwitchCount")) {
        if (mOnEventTimer != 0 && --mOnEventTimer == 0) {
            if (mVariant == 0 && !SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
                SceneflagManager::sInstance->setFlag(roomid, mActiveSceneFlag);
            } else {
                if (mVariant == 1 && !SceneflagManager::sInstance->checkBoolFlag(roomid, field_0x3BD)) {
                    SceneflagManager::sInstance->setFlag(roomid, field_0x3BD);
                }
            }
            EventManager::finishEvent(this, "OnSwitchCount");
        }
        return SUCCEEDED;
    }
    return NOT_READY;
}

int dAcOUgSwitch_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOUgSwitch_c::initializeState_Off() {
    mAnmClr.setFrame(0.0f, 0);
}
void dAcOUgSwitch_c::executeState_Off() {
    if (mVariant == 1 && SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
        mStateMgr.changeState(StateID_Active);
    }
}
void dAcOUgSwitch_c::finalizeState_Off() {}
void dAcOUgSwitch_c::initializeState_Active() {
    mAnmClr.setFrame(1.0f, 0);
}
void dAcOUgSwitch_c::executeState_Active() {
    if (mVariant == 0 && !SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
        if (isWithinPlayerRadius(sActivationRadius) && dAcPy_c::LINK->checkActionFlags(dAcPy_c::FLG0_CRAWLING)) {
            mOnEventTimer = sEventTimer;
            playSound(0xB12);
            mStateMgr.changeState(StateID_On);
        }
    }
    if (mVariant == 1) {
        if (!SceneflagManager::sInstance->checkBoolFlag(roomid, field_0x3BD)) {
            if (isWithinPlayerRadius(sActivationRadius) && dAcPy_c::LINK->checkActionFlags(dAcPy_c::FLG0_CRAWLING)) {
                mOnEventTimer = sEventTimer;
                playSound(0xB13);
                mStateMgr.changeState(StateID_On);
            }
        }
        if (!SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
            mStateMgr.changeState(StateID_Off);
        }
    }
}
void dAcOUgSwitch_c::finalizeState_Active() {}
void dAcOUgSwitch_c::initializeState_On() {
    mAnmClr.setFrame(2.0f, 0);
    static volatile u32 FLAGS_1 = 0x00000001;
    static u32 FLAGS_2 = 0x00100001;
    u32 f1 = FLAGS_1;
    u32 f2 = FLAGS_2;
    Event event = Event("OnSwitchCount", 100, f2 & ~f1, nullptr, nullptr);
    EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
}
void dAcOUgSwitch_c::executeState_On() {
    if (!SceneflagManager::sInstance->checkBoolFlag(roomid, mActiveSceneFlag)) {
        if (mVariant == 0) {
            mStateMgr.changeState(StateID_Active);
        } else {
            mStateMgr.changeState(StateID_Off);
        }
    }
}
void dAcOUgSwitch_c::finalizeState_On() {}

void dAcOUgSwitch_c::setVariant(int variant) {
    if (variant == 0) {
        mVariant = 0;
    } else {
        mVariant = 1;
    }
}
