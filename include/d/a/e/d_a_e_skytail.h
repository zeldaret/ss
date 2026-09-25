#ifndef D_A_E_SKYTAIL_H
#define D_A_E_SKYTAIL_H

#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_d3d.h"

class dAcESkytail_c : public dAcEnBase_c {
    class callbackBody_c : public m3d::callback_c {
    public:
        callbackBody_c() {}
        virtual ~callbackBody_c() {}

        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        // Node Center -> then all spines
        mVec3_c mPos[12];
        mAng3_c mAng[12];

        f32 mScaleRear;
        s16 field_0xE0; // Unused
        s16 mMouthBigAngleY;
        s16 mMouthSmallAngleY;
    };
    enum BodyNode_e {
        BODY_NODE_Center,
        BODY_NODE_Head,
        BODY_NODE_Hair,
        BODY_NODE_LMouthBig,
        BODY_NODE_LMouthSmall,
        BODY_NODE_RMouthBig,
        BODY_NODE_RMouthSmall,
        BODY_NODE_Spine01,
        BODY_NODE_Spine02,
        BODY_NODE_Spine03,
        BODY_NODE_L1wing01,
        BODY_NODE_L1wing02,
        BODY_NODE_L1wing03,
        BODY_NODE_L1wing04,
        BODY_NODE_L1wing05,
        BODY_NODE_R1wing01,
        BODY_NODE_R1wing02,
        BODY_NODE_R1wing03,
        BODY_NODE_R1wing04,
        BODY_NODE_R1wing05,
        BODY_NODE_Spine04,
        BODY_NODE_Spine05,
        BODY_NODE_L2wing01,
        BODY_NODE_L2wing02,
        BODY_NODE_L2wing03,
        BODY_NODE_L2wing04,
        BODY_NODE_R2wing01,
        BODY_NODE_R2wing02,
        BODY_NODE_R2wing03,
        BODY_NODE_R2wing04,
        BODY_NODE_Spine06,
        BODY_NODE_Spine07,
        BODY_NODE_L3wing01,
        BODY_NODE_L3wing02,
        BODY_NODE_L3wing03,
        BODY_NODE_L3wing04,
        BODY_NODE_R3wing01,
        BODY_NODE_R3wing02,
        BODY_NODE_R3wing03,
        BODY_NODE_R3wing04,
        BODY_NODE_Spine08,
        BODY_NODE_Spine09,
        BODY_NODE_Spine10,
        BODY_NODE_Spine11
    };

    class callbackAntenna_c : public m3d::callback_c {
    public:
        callbackAntenna_c() {}
        virtual ~callbackAntenna_c() {}

        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        // Corresponds to the AntennaNode_e
        mVec3_c mPos[9];
        mAng3_c mAng[9];
    };
    enum AntennaNode_e {
        ANTENNA_NODE_AntennaCenter,
        ANTENNA_NODE_Antenna01,
        ANTENNA_NODE_Antenna02,
        ANTENNA_NODE_Antenna03,
        ANTENNA_NODE_Antenna04,
        ANTENNA_NODE_Antenna05,
        ANTENNA_NODE_Antenna06,
        ANTENNA_NODE_Antenna07,
        ANTENNA_NODE_Antenna08,
    };

    enum CollisionIdx_e {
        COLLIDER_Head,
        COLLIDER_Spine4,
        COLLIDER_Spine6,
        COLLIDER_Spine8,
        COLLIDER_Rear,
    };

public:
    dAcESkytail_c() : mStateMgr(*this) {}
    virtual ~dAcESkytail_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcESkytail_c, RandomMove);
    STATE_FUNC_DECLARE(dAcESkytail_c, Move);
    STATE_FUNC_DECLARE(dAcESkytail_c, ParallelMove);
    STATE_FUNC_DECLARE(dAcESkytail_c, FrontAttack);
    STATE_FUNC_DECLARE(dAcESkytail_c, Charge);
    STATE_FUNC_DECLARE(dAcESkytail_c, Dead);
    STATE_FUNC_DECLARE(dAcESkytail_c, Skytail);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcESkytail_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcESkytail_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcESkytail_c);

    void checkHit();
    void wiggleBody();
    void wiggleAntenna(s32);
    void fn_172_45D0();
    void fn_172_45E0();
    void clampRotationX();

    // Inline not final - Called from sky enemy
    void setBirthPos(f32 x, f32 y, f32 z) {
        mBirthPos.x = x;
        mBirthPos.y = y;
        mBirthPos.z = z;
    }

private:
    /* 0x0378 */ d3d::AnmMdlWrapper mBodyMdl;
    /* 0x03E8 */ d3d::AnmMdlWrapper mAntennaMdl[2];
    /* 0x04C8 */ dBgS_AcchCir mAcchCir;
    /* 0x0524 */ dBgS_ObjAcch mAcch;
    /* 0x08D4 */ dCcD_Linked<dCcD_Sph> mCc[5];
    /* 0x0FB4 */ dColliderLinkedList mCollider;
    /* 0x0FC0 */ STATE_MGR_DECLARE(dAcESkytail_c);
    /* 0x0FFC */ callbackBody_c mBodyTransform;
    /* 0x10E4 */ callbackAntenna_c mAntennaTransform[2];
    /* 0x1234 */ mMtx_c mScratchMtx;
    /* 0x1264 */ mVec3_c mScratchVec;
    /* 0x1270 */ mVec3_c field_0x1270; // Unused
    /* 0x127C */ mVec3_c mAccel;       // Acceleration ?
    /* 0x1288 */ mVec3_c field_0x1288; // Unused
    /* 0x1294 */ mVec3_c field_0x1294; // Unused
    /* 0x12A0 */ mVec3_c field_0x12A0; // Related to Position Target?
    /* 0x12AC */ mVec3_c mParallelMoveScratchVec;
    /* 0x12B8 */ mVec3_c field_0x12B8; // Unused
    /* 0x12C4 */ mVec3_c mBirdPosition;
    /* 0x12D0 */ mVec3_c mBirthPos; // Set From sky enemy as well in BirthSkytail
    /* 0x12DC */ mVec3_c mCloseSkytailAdjustment;
    /* 0x12E8 */ mVec3_c mCloseSkytailTarget;
    /* 0x12F4 */ mVec3_c field_0x12F4; // unused
    /* 0x1300 */ mAng3_c field_0x1300; // unused
    /* 0x1306 */ mAng3_c mBirdRotation;
    /* 0x130C */ f32 field_0x130C; // Bird Related
    /* 0x1310 */ f32 field_0x1310; // unused
    /* 0x1314 */ f32 mBodyScale;
    /* 0x1318 */ f32 mRandomMoveSpeedMultiplier;
    /* 0x131C */ f32 field_0x131C; // unused
    /* 0x1310 */ f32 field_0x1320; // unused
    /* 0x1324 */ f32 mDistanceToBird;
    /* 0x1328 */ f32 field_0x1328; // unused
    /* 0x132C */ f32 field_0x132C; // unused

    /* 0x1330 */ f32 field_0x1330; // Body wiggle X amplitude
    /* 0x1334 */ f32 field_0x1334; // Body wiggle Y amplitude
    /* 0x1338 */ f32 field_0x1338; // Antenna wiggle Z amplitude
    /* 0x133C */ f32 field_0x133C; // Body wiggle Y amplitude adjustment

    /* 0x1340 */ f32 mParallelMoveRateShift;
    /* 0x1344 */ f32 mParallelMoveRateShiftTarget;

    /* 0x1348 */ f32 field_0x1348; // (Effects Velocity)
    /* 0x134C */ f32 field_0x134C; // (Effects Velocity)
    /* 0x1350 */ u8 _0x1350[0x135C - 0x1350];
    /* 0x135C */ s32 mFrontAttackSubstateTimer;
    /* 0x1360 */ s32 field_0x1360; // ParallelMove Follow Position Index?
    /* 0x1364 */ s32 mDeadSubstate;
    /* 0x1368 */ s16 mLiveTimer;    // Starts at a random value and counts up always
    /* 0x136A */ s16 field_0x136A;  // body wiggle phase X
    /* 0x136C */ s16 field_0x136C;  // body wiggle phase shift X
    /* 0x136E */ s16 field_0x136E;  // body wiggle phase Y term
    /* 0x1370 */ s16 field_0x1370;  // antenna wiggle phase term
    /* 0x1372 */ s16 field_0x1372;  // body wiggle phase Y term
    /* 0x1376 */ s16 field_0x1374;  // Phase for field_0x1348 (Effects Velocity)
    /* 0x1376 */ s16 field_0x1376;  // Phase for field_0x134C (Effects Velocity)
    /* 0x1378 */ s16 mAngleTargetY; // Angle Y Target
    /* 0x137A */ s16 mAngleTargetX; // Angle X Target
    /* 0x137C */ s16 mAngleStep;    // Angle Step
    /* 0x137E */ s16 field_0x137E;  // Angle Target Step
    /* 0x1380 */ s16 field_0x1380;  // unused

    /* 0x1382 */ s16 field_0x1382; // Phase for RotationX (Effects Velocity)
    /* 0x1384 */ s16 field_0x1384; // increment for field_0x1382
    /* 0x1386 */ s16 field_0x1386; // target angle for field_0x1384

    /* 0x1388 */ s16 mMouthSmallPhase;
    /* 0x138A */ s16 mMouthSmallPhaseIncrement; // unused
    /* 0x138C */ s16 mMouthSmallPhaseIncrementTarget;
    /* 0x138E */ s16 mMouthSmallAmplitudeAdd; // FrontAttack Related only

    /* 0x1390 */ s16 mMouthBigPhase;
    /* 0x1392 */ s16 mMouthBigPhaseIncrement; //
    /* 0x1394 */ s16 mMouthBigPhaseIncrementTarget;
    /* 0x1394 */ s16 mMouthBigAmplitudeAdd; // FrontAttack Related only

    /* 0x1398 */ s16 field_0x1398; // FrontAttack Related only
    /* 0x139A */ s16 field_0x139A; // ParallelMove Substate1 Timer
    /* 0x139A */ s16 field_0x139C; // ParallelMove something? effectively useless
    /* 0x139E */ s16 mScaleRearPhase;
    /* 0x13A0 */ s16 mDeadStateTimer;
    /* 0x13A2 */ s16 mHitCooldown;
    /* 0x13A4 */ u16 field_0x13A4[3];
    /* 0x13AA */ u16 field_0x13AA; // ParallelMove Follow Position Change Timer?

    /* 0x13AC */ u8 mParallelMoveSubstate;
    /* 0x13AD */ u8 field_0x13AD; // unused
    /* 0x13AE */ u8 field_0x13AE; // unused
    /* 0x13AF */ u8 field_0x13AF; // unused
    /* 0x13B0 */ u8 field_0x13B0; // unused
    /* 0x13B1 */ u8 mFrontAttackSubstate;
    /* 0x13B2 */ u8 field_0x13B2; // unused
    /* 0x13B3 */ u8 field_0x13B3; // unused

    /* 0x13B4 */ u8 field_0x13B4;   // ParallelMove Substate3_4 Timer
    /* 0x13B5 */ u8 field_0x13B5;   // ParallelMove SubState2 Timer
    /* 0x13B6 */ bool field_0x13B6; // Respawn from Sky Enemy Tag?
    /* 0x13B7 */ bool field_0x13B7; // Maybe Bird has Player?
    /* 0x13B8 */ bool mbNearSkytail;
};

#endif
