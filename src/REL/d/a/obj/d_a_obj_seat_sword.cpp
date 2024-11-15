#include "d/a/obj/d_a_obj_seat_sword.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_SEAT_SWORD, dAcOSeatSword_c, fProfile::OBJ_SEAT_SWORD, 0x249, 0, 3);

STATE_DEFINE(dAcOSeatSword_c, Wait);
STATE_DEFINE(dAcOSeatSword_c, Get);

dCcD_SrcCyl dAcOSeatSword_c::sCylSrc = {
  /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {0, 0x311, {0, 0x407}, 0x0, 0x0},
     /* mObjCo */ {0xE9}},
 /* mCylInf */
    {20.f, 100.f}
};

const char sEventName[] = "PickSwordSeat";

char *SwordSeatNames[] = {"SwordSeal", "SwordGrd"};

const InteractionTargetDef dAcOSeatSword_c::sInteractionDef = {
    0, 2, 1, EXAMINE_TALK, 0, 200.f, 60.f, 0.f, -100.f, 100.f, 50.f, 1.f,
};

const Vec dAcOSeatSword_c::sEffectPos = {-20.f, 260.f, 30.f};

extern "C" const char *getSwordName(int);
extern "C" u8 EQUIPPED_SWORD;
bool dAcOSeatSword_c::createHeap() {
    if (mSubtype == 0) {
        const char *goddess_sword = getSwordName(1);
        mRes = dAcPy_c::getItemResFile(goddess_sword, heap_allocator);
        TRY_CREATE(mSwordMdl.create(mRes.GetResMdl(goddess_sword), &heap_allocator, 0x120, 1, nullptr));
    } else if (mSubtype == 1) {
        mRes = dAcPy_c::LINK->mSwordRes;
        const char *sword_name = getSwordName(EQUIPPED_SWORD);
        TRY_CREATE(mSwordMdl.create(mRes.GetResMdl(sword_name), &heap_allocator, 0x120, 1, nullptr));
    }

    nw4r::g3d::ResFile mPedRes = getOarcResFile(SwordSeatNames[mSubtype]);
    TRY_CREATE(mPedestalMdl.create(mPedRes.GetResMdl(SwordSeatNames[mSubtype]), &heap_allocator, 0x120, 1, nullptr));
    void *dzb = getOarcDZB(SwordSeatNames[mSubtype], SwordSeatNames[mSubtype]);
    void *plc = getOarcPLC(SwordSeatNames[mSubtype], SwordSeatNames[mSubtype]);
    updateMatrix();
    mSwordMtx.copyFrom(mWorldMtx);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale));
    if (mSubtype == 1) {
        TRY_CREATE(mPedestalMdl_s.create(mPedRes.GetResMdl("SwordGrd_s"), &heap_allocator, 0x120, 1, nullptr));
    }

    return true;
}
const s16 l_inGroundRot = {0x71C};
int dAcOSeatSword_c::create() {
    mSubtype = getSubtype();
    if (mSubtype >= 2) {
        mSubtype = 0;
    }

    CREATE_ALLOCATOR(dAcOSeatSword_c);

    mStts.SetRank(0xD);
    mCyl.Set(sCylSrc);
    mCyl.SetStts(mStts);
    dBgS::GetInstance()->Regist(&mBgW, this);

    mPedestalMdl.setLocalMtx(mWorldMtx);
    if (mSubtype == 1) {
        mPedestalMdl_s.setLocalMtx(mWorldMtx);
        mPedestalMdl_s.setPriorityDraw(0x22, 0x9);
    }

    forwardAccel = 0.f;
    forwardMaxSpeed = -40.f;

    if (mSubtype == 1) {
        mField_0x7E8 = l_inGroundRot;
    }
    updateSwordMdl();
    mEff.init(this);
    mPedestalMdl.setPriorityDraw(0x1C, 0x9);

    if (checkPlayerHasSword()) {
        mStateMgr.changeState(StateID_Get);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    boundingBox.Set(mVec3_c(-75.f, -10.f, -75.f), mVec3_c(75.f, 200.f, 75.f));

    return SUCCEEDED;
}

int dAcOSeatSword_c::doDelete() {
    return SUCCEEDED;
}

int dAcOSeatSword_c::actorExecute() {
    mStateMgr.executeState();

    if (mStateMgr.isState(StateID_Wait)) {
        actorExecuteCommon();
        mCyl.SetC(position);
        dCcS::GetInstance()->Set(&mCyl);
    }

    return SUCCEEDED;
}

int dAcOSeatSword_c::actorExecuteInEvent() {
    if (mbNoSword) {
        sLib::chase(&mField_0x7E4, 255.f, 8.f);
    }
    actorExecuteCommon();

    if (mEvent.isThisActorInEvent()) {
        switch (mEvent.getCurrentEventCommand()) {
            case 'act0': {
                mEvent.advanceNext();
            } break;
            case 'act1': {
                dAcPy_c::LINK->getBodyMtx(&mSwordMtx, 0xF /* weaponR*/);
                mSwordMdl.setLocalMtx(mSwordMtx);
                mEvent.advanceNext();
            } break;
            case 'act2': {
                mbNoSword = true;
                dAcPy_c::LINK->getBodyMtx(&mSwordMtx, 0xF /* weaponR*/);
                mSwordMdl.setLocalMtx(mSwordMtx);
                mEvent.advanceNext();
            }
        }
    }

    return SUCCEEDED;
}

void dAcOSeatSword_c::doInteraction(s32 param0) {
    if (param0 == 5) {
        mField_0x7EA = true;
        Event evt((char *)sEventName, 0x190, 0x100001, nullptr, nullptr);
        mEvent.scheduleEvent(evt, 0);
    } else {
        mField_0x7EA = false;
    }
}

u32 someAng = 0x8000;
void dAcOSeatSword_c::registerInEvent() {
    static const u32 rodata_stuff = {0};
    if (mField_0x7EA == 1 && EventManager::isCurrentEvent(sEventName)) {
        dAcPy_c *player = dAcPy_c::LINK;
        mVec3_c vec = mVec3_c::Ez * 78.f;
        mAng3_c ang = rotation;
        ang.y += someAng;
        vec.rotY(rotation.y);
        vec += position;
        player->setPosRot(vec, ang, 0, 1, 0);
        mField_0x7E8 = 0;
        updateSwordMdl();
    }
}

int dAcOSeatSword_c::draw() {
    if (!mStateMgr.isState(StateID_Get)) {
        drawModelType1(&mSwordMdl);
    }

    drawModelType1(&mPedestalMdl);
    if (mSubtype == 1) {
        drawModelType1(&mPedestalMdl_s);
    }

    return SUCCEEDED;
}

void dAcOSeatSword_c::initializeState_Wait() {}
void dAcOSeatSword_c::executeState_Wait() {
    AttentionManager::sInstance->addTarget(*this, sInteractionDef, 0, nullptr);

    if (checkPlayerHasSword()) {
        mbNoSword = true;
    }
    if (mbNoSword) {
        mStateMgr.changeState(StateID_Get);
    }
}
void dAcOSeatSword_c::finalizeState_Wait() {}

// Get state to remove any interaction/effect
void dAcOSeatSword_c::initializeState_Get() {
    mEff.remove(true);
}
void dAcOSeatSword_c::executeState_Get() {}
void dAcOSeatSword_c::finalizeState_Get() {}

extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_76_;
void dAcOSeatSword_c::actorExecuteCommon() {
    if (checkPlayerHasSword()) {
        return;
    }

    // ??????? this func

    const dAcPy_c *player = dAcPy_c::LINK;

    mEffPos.x = sEffectPos.x;
    mEffPos.y = sEffectPos.y;
    mEffPos.z = sEffectPos.z;

    mEffPos.rotY(player->rotation.y);
    f32 y = player->position.y;

    mEffPos += player->position;
    mEffPos.y = y;

    mEff.fn_80029A10(PARTICLE_RESOURCE_ID_MAPPING_76_, &mEffPos, &rotation, &mScale, nullptr, nullptr);
    mEff.fn_80027320(mField_0x7E4);
}

void dAcOSeatSword_c::updateSwordMdl() {
    updateMatrix();
    mSwordMtx.copyFrom(mWorldMtx);

    if (mSubtype == 1) {
        mSwordMtx.XrotM(mField_0x7E8);
    }

    mMtx_c mtx;
    // Weird value for y, but ok
    mtx.transS(0.f, 87.51f, 0.f);

    mSwordMtx += mtx;
    mSwordMtx.YrotM(0x4000);
    mSwordMtx.ZrotM(0xC000);
    mSwordMdl.setLocalMtx(mSwordMtx);
}

bool dAcOSeatSword_c::checkPlayerHasSword() {
    // the GetFlag needs to be an inline ... (using getCounterOrFlag doesnt work)
    return (mSubtype == 0 && StoryflagManager::sInstance->getFlag(0x3)) ||
           (mSubtype == 1 && !StoryflagManager::sInstance->getFlag(0xA7));
}
