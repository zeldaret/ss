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

        f32 mScaleSpine11;
        s16 field_0x10DC;
        s16 field_0x10DE;
        s16 field_0x10E0;
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

    void fn_172_A40();
    void fn_172_3CD0();
    void fn_172_4240(s32);
    void fn_172_45D0();
    void fn_172_45E0();
    void fn_172_45F0();

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
    /* 0x1234 */ mMtx_c field_0x1234;
    /* 0x1264 */ mVec3_c field_0x1264;
    /* 0x1270 */ mVec3_c field_0x1270;
    /* 0x127C */ mVec3_c field_0x127C;
    /* 0x1288 */ mVec3_c field_0x1288;
    /* 0x1294 */ mVec3_c field_0x1294;
    /* 0x12A0 */ mVec3_c field_0x12A0;
    /* 0x12AC */ mVec3_c field_0x12AC;
    /* 0x12B8 */ mVec3_c field_0x12B8;
    /* 0x12C4 */ mVec3_c mBirdPosition;
    /* 0x12D0 */ mVec3_c field_0x12D0;
    /* 0x12DC */ mVec3_c field_0x12DC;
    /* 0x12E8 */ mVec3_c field_0x12E8;
    /* 0x12F4 */ mVec3_c field_0x12F4;
    /* 0x1300 */ mAng3_c field_0x1300;
    /* 0x1306 */ mAng3_c mBirdRotation;
    /* 0x130C */ f32 field_0x130C;
    /* 0x1310 */ f32 field_0x1310;
    /* 0x1314 */ f32 mScaleF;
    /* 0x1318 */ f32 field_0x1318;
    /* 0x131C */ f32 field_0x131C;
    /* 0x1310 */ f32 field_0x1320;
    /* 0x1324 */ f32 mDistanceToBird;
    /* 0x1328 */ f32 field_0x1328;
    /* 0x132C */ f32 field_0x132C;
    /* 0x1330 */ f32 field_0x1330;
    /* 0x1334 */ f32 field_0x1334;
    /* 0x1338 */ f32 field_0x1338;
    /* 0x133C */ f32 field_0x133C;
    /* 0x1340 */ f32 field_0x1340;
    /* 0x1344 */ f32 field_0x1344;
    /* 0x1348 */ f32 field_0x1348;
    /* 0x134C */ f32 field_0x134C;
    /* 0x1350 */ u8 _0x1350[0x135C - 0x1350];
    /* 0x135C */ s32 field_0x135C;
    /* 0x1360 */ s32 field_0x1360;
    /* 0x1364 */ s32 field_0x1364;
    /* 0x1368 */ s16 field_0x1368;
    /* 0x136A */ s16 field_0x136A;
    /* 0x136C */ s16 field_0x136C;
    /* 0x136E */ s16 field_0x136E;
    /* 0x1370 */ s16 field_0x1370;
    /* 0x1372 */ s16 field_0x1372;
    /* 0x1376 */ s16 field_0x1374;
    /* 0x1376 */ s16 field_0x1376;
    /* 0x1378 */ s16 field_0x1378;
    /* 0x137A */ s16 field_0x137A;
    /* 0x137C */ s16 field_0x137C;
    /* 0x137E */ s16 field_0x137E;
    /* 0x1380 */ s16 field_0x1380;
    /* 0x1382 */ s16 field_0x1382;
    /* 0x1384 */ s16 field_0x1384;
    /* 0x1386 */ s16 field_0x1386;
    /* 0x1388 */ s16 field_0x1388;
    /* 0x1388 */ s16 field_0x138A;
    /* 0x138C */ s16 field_0x138C;
    /* 0x138E */ s16 field_0x138E;
    /* 0x1390 */ s16 field_0x1390;
    /* 0x1392 */ s16 field_0x1392;
    /* 0x1394 */ s16 field_0x1394;
    /* 0x1394 */ s16 field_0x1396;
    /* 0x1398 */ s16 field_0x1398;
    /* 0x139A */ s16 field_0x139A;
    /* 0x139A */ s16 field_0x139C;
    /* 0x139E */ s16 field_0x139E;
    /* 0x13A0 */ s16 field_0x13A0;
    /* 0x13A2 */ s16 field_0x13A2;
    /* 0x13A4 */ u16 field_0x13A4[3];
    /* 0x13AA */ u16 field_0x13AA;
    /* 0x13AC */ u8 field_0x13AC;
    /* 0x13AD */ u8 field_0x13AD;
    /* 0x13AE */ u8 field_0x13AE;
    /* 0x13AF */ u8 field_0x13AF;
    /* 0x13B0 */ u8 field_0x13B0;
    /* 0x13B1 */ u8 field_0x13B1;
    /* 0x13B2 */ u8 field_0x13B2;
    /* 0x13B3 */ u8 field_0x13B3;
    /* 0x13B4 */ u8 field_0x13B4;
    /* 0x13B5 */ u8 field_0x13B5;
    /* 0x13B6 */ u8 field_0x13B6;
    /* 0x13B7 */ bool field_0x13B7; // Maybe Bird has Player?
    /* 0x13B8 */ u8 field_0x13B8;
};

#endif
