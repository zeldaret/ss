#include "d/a/obj/d_a_obj_dungeon_ship.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmclr.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_DUNGEON_SHIP, dAcODungeonShip_c, fProfile::OBJ_DUNGEON_SHIP, 0x17B, 0, 3);

dCcD_SrcCps dAcODungeonShip_c::sCcSrc = {
    {

     {0, 0x1003E, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, 0, 0x407, 0, 0},
     {0xE8},
     },
    {1000.0f},
};

STATE_DEFINE(dAcODungeonShip_c, Wait);
STATE_DEFINE(dAcODungeonShip_c, Transparency);
STATE_DEFINE(dAcODungeonShip_c, AppearEvent);
STATE_DEFINE(dAcODungeonShip_c, Appear);
STATE_DEFINE(dAcODungeonShip_c, End);

const f32 dAcODungeonShip_c::someFloats[18] = {
    1.0f,   1.0f,   0.0f, 500.0f, 0.0f, 20.0f,   50.0f,   30.0f, 100.0f,
    150.0f, 100.0f, 0.0f, 0.0f,   0.0f, 4000.0f, 2000.0f, 0.0f,  0.0f,
};

void dAcODungeonShip_c::eventIn_Wrapper(void *arg) {
    if (arg != nullptr) {
        static_cast<dAcODungeonShip_c *>(arg)->eventIn();
    }
}

void dAcODungeonShip_c::eventEnd_Wrapper(void *arg) {
    if (arg != nullptr) {
        static_cast<dAcODungeonShip_c *>(arg)->eventEnd();
    }
}

bool dAcODungeonShip_c::createHeap() {
    const char *arcName = "ShipDungeon";
    mRes = nw4r::g3d::ResFile(getOarcResFile(arcName));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("ShipDungeonN");
    if (!mMdl.fn_8001F3B0(mdl, &heap_allocator, 0x120)) {
        return false;
    }

    nw4r::g3d::ResAnmChr anmChr = mRes.GetResAnmChr("ShipDungeonN");
    if (!mAnmChr.create(mdl, anmChr, &heap_allocator, nullptr)) {
        return false;
    }
    mMdl.setAnm(mAnmChr);

    nw4r::g3d::ResAnmClr anmClr = mRes.GetResAnmClr("ShipDungeonN");
    if (!mAnmMatClr.create(mdl, anmClr, &heap_allocator, nullptr, 1)) {
        return false;
    }

    mAnmMatClr.setRate(0.0f, 0);
    mAnmChr.setRate(0.0f);
    mMdl.setPriorityDraw(0x7F, 4);

    const char *dzbPlcName = "ShipDungeonN";
    void *dzb = getOarcDZB(arcName, dzbPlcName);
    void *plc = getOarcPLC(arcName, dzbPlcName);
    updateMatrix();
    if (mBg.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale)) {
        return false;
    }

    // wat
    if (!SceneflagManager::sInstance->checkBoolFlag(roomid, (params >> 0x10) & 0xFF)) {
        goto ok;
    } else {
        bool result = (BOOL)mBg.InitMapStuff(&heap_allocator);
        if (result == false) {
            return false;
        }
    }
ok:
    return true;
}

#pragma explicit_zero_data on
static u32 rotX_init = 0;
#pragma explicit_zero_data off

int dAcODungeonShip_c::create() {
    if (!StoryflagManager::sInstance->getCounterOrFlag(271)) {
        return FAILED;
    }

    if (SceneflagManager::sInstance->checkBoolFlag(roomid, (params >> 0x10) & 0xFF)) {
        mIsDocked = true;
    }

    if (!initAllocatorWork1Heap(0x40000, "dAcODungeonShip_c::m_allocator", 0x20)) {
        return FAILED;
    }

    mPathIdx = params & 0xFF;
    if (mPathIdx == 0xFF) {
        return FAILED;
    }

    dBgS::GetInstance()->Regist(&mBg, this);
    mStts.SetRank(2);
    mCc.Set(sCcSrc);
    mCc.SetStts(mStts);
    forwardAccel = 0.0f;
    forwardMaxSpeed = 0.0f;
    boundingBox.Set(mVec3_c(-6000.0f, -500.0f, -2000.0f), mVec3_c(5000.0f, 4000.0f, 2000.0f));
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, (params >> 0x10) & 0xFF)) {
        dBgS::GetInstance()->RegistBg(&mBg, this);
        mStateMgr.changeState(StateID_End);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    mEffects[0].init(this);
    field_0x8D8 = 0;
    mEffects[1].init(this);
    field_0x8D9 = 0;

    if (!SceneflagManager::sInstance->checkBoolFlag(roomid, (params >> 0x10) & 0xFF)) {
        mDowsingOffset.set(0.0f, 500.0f, 0.0f);
        mDowsingTarget.initialize(DowsingTarget::SLOT_STORY_EVENT, 0, &mDowsingOffset, 10000.0);
        mDowsingTarget.doRegister();
        mIsRoaming = true;
    }

    mCullingDistance = 200000.0f;
    clearActorProperty(0x1);
    mAppearEventFromParam = (params >> 0x18);
    field_0x849 = rotation.x;
    rotation.x = rotX_init;
    updateMatrix();
    mBg.Move();
    return SUCCEEDED;
}

int dAcODungeonShip_c::doDelete() {
    if (mIsRoaming) {
        mDowsingTarget.doUnregister();
    }
    return SUCCEEDED;
}

extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_682_;
extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_683_;

int dAcODungeonShip_c::actorExecute() {
    field_0x850++;
    field_0x86C++;
    mStateMgr.executeState();

    // I hate whatever this stupid pattern in actors is with
    // redundant casts and adding 0.0f to stuff
    int tempZero = 0;
    mVec3_c offsetPosition(position.x, position.y + 1200.0f + tempZero, position.z);

    f32 scale = tempZero + 2100.0f;
    mVec3_c directedScale = mVec3_c::Ex * scale;
    directedScale.rotY(rotation.y);

    f32 extent = tempZero + 1800.0f;
    mCc.Set(offsetPosition + directedScale, offsetPosition - directedScale, extent);

    dCcS::GetInstance()->Set(&mCc);
    mMdl.play();
    mAnmMatClr.play();
    mMdl.calc(false);
    if (field_0x8D8) {
        mEffects[0].fn_80029A10(PARTICLE_RESOURCE_ID_MAPPING_682_, &position, &rotation, nullptr, nullptr, nullptr);
    }

    if (field_0x8D9) {
        mMtx_c tmpMtx;
        mMdl.getNodeWorldMtx(0, tmpMtx);
        if (mEffects[1].checkField0x00()) {
            mEffects[1].setMtx(tmpMtx);
        } else if (field_0x8DA == 0) {
            mEffects[1].fn_800299F0(PARTICLE_RESOURCE_ID_MAPPING_683_, &tmpMtx, nullptr, nullptr);
            field_0x8DA = 1;
        }
    }
    updateMatrixAndScale();
    mBg.Move();
    return SUCCEEDED;
}

int dAcODungeonShip_c::draw() {
    mMdl.mProc.mCallback.field_0x08 = field_0x862;
    mMdl.mProc.mCallback.field_0x09 = 1;
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcODungeonShip_c::initializeState_Wait() {
    fn_485_1660();
}
void dAcODungeonShip_c::executeState_Wait() {
    if (mCc.ChkTgHit() && mCc.ChkTgAtHitType(AT_TYPE_BOMB)) {
        mNumTimesHit++;
        field_0x862 = 0x8C;
        field_0x863 = 0;
        field_0x864 = 0x8C;
        mAppearEvent = field_0x849;
        playSound(0xB62);
        mStateMgr.changeState(StateID_AppearEvent);
    } else {
        fn_485_1BF0();
    }
}
void dAcODungeonShip_c::finalizeState_Wait() {}

void dAcODungeonShip_c::initializeState_Transparency() {
    field_0x862 = 0;
}
void dAcODungeonShip_c::executeState_Transparency() {
    if (mCc.ChkTgHit() && mCc.ChkTgAtHitType(AT_TYPE_BOMB)) {
        if (mNumTimesHit == 1) {
            field_0x862 = 0xA0;
            field_0x863 = 0;
            field_0x855 = 0;
            mNumTimesHit++;
            playSound(0xB62);
        } else if (mNumTimesHit == 2) {
            field_0x863 = 0;
            mNumTimesHit++;
            SceneflagManager::sInstance->setFlag(roomid, (params >> 0x10) & 0xFF);
            mAppearEvent = mAppearEventFromParam;
            field_0x8D8 = 1;
            mStateMgr.changeState(StateID_AppearEvent);
            playSound(0xB62);
            return;
        }
    }

    if (mNumTimesHit == 2) {
        if (field_0x855 < 0x28) {
            field_0x855++;
        }
        f32 cos = nw4r::math::CosIdx((field_0x855 * 0x199));
        if (cos < 0.0f) {
            cos = 0.0f;
        }
        field_0x862 = cos * 160.0f;
    } else {
        sLib::chaseUC(&field_0x862, field_0x863, 8);
    }
    field_0x858 = 20.0f;

    const dAcPy_c *link = dAcPy_c::GetLink();
    if (link != nullptr) {
        if (field_0x868 > 0) {
            field_0x868--;
        }

        // f32 distToLink = getSquareDistanceTo(link->position);
        f32 dist2 = 100000000.0f;
        f32 dist1 = 225000000.0f;
        bool isWithinDist2 = getSquareDistanceTo(link->position) < dist2;
        bool isWithinDist1 = getSquareDistanceTo(link->position) < dist1;
        s32 tmp1 = fn_485_1960();
        bool tmp2 = tmp1 < 0x1555;
        if (isWithinDist1 && field_0x868 == 0 && field_0x862 == 0 && tmp2) {
            if (field_0x856 > 0) {
                field_0x856 = 0xFF;
                mPath.SetFlag(0x40000000);
            } else {
                field_0x856 = 1;
                mPath.ClearFlag(0x40000000);
            }

            u16 idx = fn_485_1900();
            if (idx != 0xFFFF) {
                mPath.setSegment(idx, 1.0f - mPath.getSegmentFraction());
                field_0x868 = 900;
            }
        }

        if (isWithinDist2 && !tmp2) {
            field_0x858 = 50.0f;
        }

        if (mPath.CheckFlag(0x40000000) && field_0x858 > 0.0f) {
            field_0x858 *= -1;
        }
    }
    sLib::addCalc(&forwardSpeed, field_0x858, 0.02f, 1.0f, 0.1f);
    fn_485_1720();
}
void dAcODungeonShip_c::finalizeState_Transparency() {}

void dAcODungeonShip_c::initializeState_AppearEvent() {
    if (mAppearEvent != 0xFF) {
        Event ev(mAppearEvent, roomid, 0x100001, (void *)&eventIn_Wrapper, (void *)&eventEnd_Wrapper);
        mEvent.scheduleEvent(ev, 0);
    }
}
void dAcODungeonShip_c::executeState_AppearEvent() {
    if (mAppearEvent != 0xFF) {
        if (mEvent.isThisActorInEvent()) {
            if (mAppearEvent == mAppearEventFromParam) {
                field_0x8D9 = 1;
            }

            switch (mEvent.getCurrentEventCommand()) {
                case 'jmap':
                case 'wait':
                    mEvent.isAdvance();
                    mEvent.advanceNext();
                    break;
                case 'app2':
                    if (mEvent.isAdvance()) {
                        mAnmChr.setRate(1.0f);
                        mAnmChr.setPlayState(m3d::PLAY_MODE_1);
                        mAnmMatClr.setRate(1.0f, 0);
                    } else {
                        mEvent.advanceNext();
                    }
                    fn_485_1EE0();
                    break;
                case 'appe':
                    if (mEvent.isAdvance()) {
                        mAnmChr.setRate(1.0f);
                        mAnmChr.setPlayState(m3d::PLAY_MODE_1);
                        mAnmMatClr.setFrame(0.0f, 0);
                        mAnmMatClr.setRate(1.0f, 0);
                    } else if (mAnmChr.isStop()) {
                        mEvent.advanceNext();
                    }
                    fn_485_1EE0();
                    break;
                case 'app1':
                    sLib::chaseUC(&field_0x862, field_0x863, 2);
                    if (field_0x862 == field_0x863) {
                        mEvent.advanceNext();
                    }
                    fn_485_1DF0();
                    break;
                case 'init':
                    if (mEvent.isAdvance()) {
                        f32 f;
                        if (mEvent.getSingleFloatData(&f, 'ang0', 0) == 1) {
                            mAng ang = mAng::fromDeg(f);
                            rotation.y = ang;
                        }
                        mVec3_c vec;
                        if (mEvent.getSingleVecData(&vec, 'pos0', 0) == 1) {
                            position = vec;
                        }
                        field_0x8D8 = 0;
                    }
                    mEvent.advanceNext();
                    break;
                default: mEvent.advanceNext(); break;
            }
        } else {
            Event ev(mAppearEvent, roomid, 0x100001, (void *)&eventIn_Wrapper, (void *)&eventEnd_Wrapper);
            mEvent.scheduleEvent(ev, 0);
        }
    } else {
        sLib::chaseUC(&field_0x862, field_0x863, 3);
        if (field_0x862 == field_0x863) {
            if (mAppearEvent == mAppearEventFromParam) {
                mStateMgr.changeState(StateID_Appear);
            } else {
                mStateMgr.changeState(StateID_Transparency);
            }
        }
    }
}
void dAcODungeonShip_c::finalizeState_AppearEvent() {}

void dAcODungeonShip_c::initializeState_Appear() {
    field_0x862 = 0xFF;
}
void dAcODungeonShip_c::executeState_Appear() {
    if (!mIsDocked) {
        fn_485_1A90();
    }
}
void dAcODungeonShip_c::finalizeState_Appear() {}

void dAcODungeonShip_c::initializeState_End() {
    field_0x862 = 0xFF;
}
void dAcODungeonShip_c::executeState_End() {
    if (!mIsDocked) {
        fn_485_1A90();
    }
}
void dAcODungeonShip_c::finalizeState_End() {}

void dAcODungeonShip_c::eventIn() {}
void dAcODungeonShip_c::eventEnd() {
    if (mNumTimesHit == 1) {
        mStateMgr.changeState(StateID_Transparency);
    } else {
        mStateMgr.changeState(StateID_Appear);
    }
}

void dAcODungeonShip_c::fn_485_1660() {
    f32 arg = 0.0f;
    f32 arg2 = 0.0001f;
    field_0x856 = 1;
    if (mPath.initExt(mPathIdx, roomid, 0, 0, 0, arg, arg, arg2)) {
        mPath.setSegment(0, arg);
        position = mPath.getPosition();
        mOldPosition = mPath.getPosition();
    }
}

static u32 rot_7fff = 0x7FFF;
static u32 rot_4000 = 0x4000;

void dAcODungeonShip_c::fn_485_1720() {
    mPath.setSpeed(forwardSpeed);
    mPath.fn_800A9650();
    // TODO
    position = mPath.getPosition();

    mVec3_c tmp;
    mPath.fn_800A7C80(mPath.getSegmentIndex(), tmp, mPath.getSegmentFraction());
    rotation.y = cM::atan2s(tmp.x, tmp.z);
    if (mPath.CheckFlag(0x40000000)) {
        rotation.y += rot_7fff;
    }
    rotation.y += rot_4000;
    angle.y = rotation.y;

    int factor = 0x12C;
    f32 tmp2 = nw4r::math::SinIdx((field_0x850 * 800));
    tmp2 *= factor;
    field_0x84C = tmp2;
}

bool dAcODungeonShip_c::fn_485_1830(s32 pathSegmentIndex, s32 direction, s32 *out) {

}

u16 dAcODungeonShip_c::fn_485_1900() {
    s32 sign = mPath.CheckFlag(0x40000000) ? -1 : 1;
    s32 result;
    if (fn_485_1830(mPath.getSegmentIndex(), sign, &result)) {
        return result;
    } else {
        return -1;
    }
}

u32 dAcODungeonShip_c::fn_485_1960() {
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (link == nullptr) {
        return 0;
    }
    mVec3_c v = mVec3_c::Ez;
    v.rotY(angle.y + mAng(-0x4000));
    mVec3_c dist = link->position - position;
    dist.y = 0.0f;
    dist.normalizeRS();
    int a1 = cLib::targetAngleY(mVec3_c::Zero, v);
    int a2 = cLib::targetAngleY(mVec3_c::Zero, dist);
    // okay
    return labs(mAng(mAng(a1) - mAng(a2)));
}

f32 dAcODungeonShip_c::fn_485_1A50() {
    return 4000.0f;
}

f32 dAcODungeonShip_c::fn_485_1A60() {
    return 2000.0f;
}

void dAcODungeonShip_c::fn_485_1A70(mVec3_c *v) {
    v->set(0.0f, 0.0f, 0.0f);
}

void dAcODungeonShip_c::fn_485_1A90() {
    
}

void dAcODungeonShip_c::fn_485_1BF0() {
    
}

void dAcODungeonShip_c::fn_485_1DF0() {
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (link != nullptr) {
        // unused, stack problems
        mVec3_c dist = position - link->position;
        fn_485_1960();
        field_0x858 = 30.0f;
        sLib::addCalc(&forwardSpeed, 30.0f, 0.02f, 1.0f, 0.1f);
        fn_485_1720();
    }
}

void dAcODungeonShip_c::updateMatrixAndScale() {
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);
}

void dAcODungeonShip_c::fn_485_1EE0() {
    if (cM::isZero(mAnmMatClr.getRate(0))) {
        return;
    }

    nw4r::g3d::AnmObjMatClrRes *nw4rObj = mAnmMatClr.getChild(0);
    nw4r::g3d::ClrAnmResult result;
    field_0x862 = nw4rObj->GetResult(&result, 0)->rgba[nw4r::g3d::ClrAnmResult::CLA_TEV1];
}

void unusedFloats() {
    // these are probably not floats but I have no idea why this data is here
    2.24E-43f;
    5.6E-44f;
}
