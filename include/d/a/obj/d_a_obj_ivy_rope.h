#ifndef D_A_OBJ_IVY_ROPE_H
#define D_A_OBJ_IVY_ROPE_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_rope_model.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/actor_on_rail.h"
#include "toBeSorted/attention.h"

class dAcOivyRope_c : public dAcObjBase_c {
public:
    dAcOivyRope_c() : mField_0x330(0), mStateMgr(*this, sStateID::null), mStts2(this), mEvent(*this, nullptr) {}
    virtual ~dAcOivyRope_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void registerInEvent() override;
    virtual bool canBeLinkedToWoodTag() override;
    virtual bool drop() override;

    STATE_FUNC_DECLARE(dAcOivyRope_c, RopeWait);
    STATE_FUNC_DECLARE(dAcOivyRope_c, PlayerGrip);
    STATE_FUNC_DECLARE(dAcOivyRope_c, RopeReturn);
    STATE_FUNC_DECLARE(dAcOivyRope_c, RopeCut);
    STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeWait);
    STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeUp);
    STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeDown);
    STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeInit);
    STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopePlayerCarry);

    void initTightRope();
    UNKTYPE fn_256_10C0(UNKTYPE);
    UNKTYPE fn_256_11A0(UNKTYPE);
    UNKTYPE fn_256_1240(UNKTYPE);
    UNKTYPE fn_256_1480(UNKTYPE);
    UNKTYPE fn_256_2160(UNKTYPE);
    UNKTYPE fn_256_26B0(UNKTYPE);
    UNKTYPE fn_256_2C40(UNKTYPE);
    UNKTYPE fn_256_2D00(UNKTYPE);
    UNKTYPE fn_256_2DC0(UNKTYPE);
    UNKTYPE fn_256_2EA0(UNKTYPE);
    UNKTYPE fn_256_30A0(UNKTYPE);
    UNKTYPE fn_256_33B0(UNKTYPE);
    UNKTYPE fn_256_34B0(UNKTYPE);
    UNKTYPE fn_256_3540(UNKTYPE);
    UNKTYPE fn_256_35E0(UNKTYPE);
    UNKTYPE fn_256_36A0(UNKTYPE);
    UNKTYPE fn_256_3770(UNKTYPE);
    UNKTYPE fn_256_3A20(UNKTYPE);
    UNKTYPE fn_256_3B90(UNKTYPE);
    UNKTYPE calcDistance(UNKTYPE);
    UNKTYPE fn_256_3E30(UNKTYPE);
    UNKTYPE fn_256_3E70(UNKTYPE);
    UNKTYPE fn_256_3F60(UNKTYPE);
    UNKTYPE fn_256_43B0(UNKTYPE);
    UNKTYPE fn_256_4520(UNKTYPE);
    UNKTYPE fn_256_4760(UNKTYPE);
    UNKTYPE fn_256_4C80(UNKTYPE);
    UNKTYPE fn_256_4CA0(UNKTYPE);
    int createBase();
    UNKTYPE fn_256_5190(UNKTYPE);
    UNKTYPE fn_256_5590(UNKTYPE);
    UNKTYPE fn_256_57F0(UNKTYPE);
    UNKTYPE fn_256_5B00(UNKTYPE);
    UNKTYPE fn_256_5B64(UNKTYPE);
    UNKTYPE fn_256_5B70(UNKTYPE);
    UNKTYPE fn_256_5DB0(UNKTYPE);
    UNKTYPE fn_256_5FF0(UNKTYPE);
    UNKTYPE fn_256_6100(UNKTYPE);
    UNKTYPE fn_256_61F0(UNKTYPE);
    UNKTYPE fn_256_7300(UNKTYPE);
    UNKTYPE fn_256_7310(UNKTYPE);
    UNKTYPE fn_256_73D0(UNKTYPE);
    UNKTYPE fn_256_7560(UNKTYPE);
    UNKTYPE fn_256_7680(UNKTYPE);
    UNKTYPE fn_256_7D50(UNKTYPE);
    UNKTYPE fn_256_8060(UNKTYPE);
    UNKTYPE fn_256_8590(UNKTYPE);
    UNKTYPE fn_256_8990(UNKTYPE);
    UNKTYPE fn_256_8D10(UNKTYPE);
    void fn_256_9120(u16);
    UNKTYPE fn_256_9280(UNKTYPE);
    UNKTYPE fn_256_9450(UNKTYPE);
    UNKTYPE fn_256_9C80(UNKTYPE);
    UNKTYPE fn_256_A040(UNKTYPE);
    void fn_256_A2C0(bool);
    bool fn_256_A750(bool bool0, f32 float0);
    UNKTYPE fn_256_AA40(UNKTYPE);
    UNKTYPE fn_256_AAB0(UNKTYPE);
    UNKTYPE fn_256_AAF0(UNKTYPE);
    UNKTYPE fn_256_ABA0(UNKTYPE);
    UNKTYPE fn_256_AC00(UNKTYPE);
    UNKTYPE fn_256_AE00(UNKTYPE);
    UNKTYPE fn_256_B2B0(UNKTYPE);
    UNKTYPE fn_256_BAB0(UNKTYPE);
    UNKTYPE fn_256_BB70(UNKTYPE);
    UNKTYPE fn_256_BE80(UNKTYPE);
    UNKTYPE fn_256_BFF0(UNKTYPE);
    UNKTYPE fn_256_C200(UNKTYPE);
    void fn_256_C410();
    bool fn_256_C6F0();
    void fn_256_C740();
    void fn_256_C810(mVec3_c &pOut, int idx);
    void fn_256_C960();
    void fn_256_C980(f32, f32);
    UNKTYPE fn_256_C9B0(UNKTYPE);
    void fn_256_CD40();
    void fn_256_CE20(f32 *, f32 *, f32 *);
    void fn_256_CFA0(bool);
    void fn_256_D050();
    void fn_256_D110();
    void fn_256_D1B0();
    void fn_256_D2B0();
    void fn_256_D3D0(mVec3_c &pOut1, mVec3_c &pOut2, s16 param2, bool bool0, f32 float0);
    const mVec3_c &fn_256_D730(s32 idx);    // Get Path Point
    void fn_256_D7A0(s32, const mVec3_c &); // Set Path Point
    void fn_256_D850();                     // Draw Shadow
    void fn_256_DAA0(bool, bool, f32, f32);
    void fn_256_DE80();
    void fn_256_DEE0();
    u32 fn_256_DF30();
    void fn_256_E3E0();
    bool fn_256_E790();

    bool checkSubtype(u32 sub) {
        return mSubtype == sub;
    }

private:
    /* 0x 330 */ UNKWORD mField_0x330;
    /* 0x 334 */ m3d::smdl_c mMdlArr[2];
    /* 0x 36C */ dShadowCircle_c mShadow;
    /* 0x 374 */ dCcD_Sph mSph;
    /* 0x 4C4 */ dCcD_Cps mCps1;
    /* 0x 634 */ dCcD_Cps mCps2;
    /* 0x 7A4 */ dCcD_Cps mCpsArr[3];
    /* 0x BF4 */ dCcD_Cyl mCyl;
    /* 0x D44 */ STATE_MGR_DECLARE(dAcOivyRope_c);
    /* 0x D80 */ dMultiMat_c mRopeMdl;
    /* 0x DCC */ dSkinMat_c mCoilMdl;
    /* 0x E24 */ dMultiMat_c mRopeMdl2;
    /* 0x E70 */ ActorOnRail3 mPath;
    /* 0x E88 */ u8 _E88[0xE94 - 0xE88];
    /* 0x E94 */ cCcD_Stts mStts2;
    /* 0x ED0 */ ActorEventRelated mEvent;
    /* 0x F20 */ mVec3_c mTightropeEnd;
    /* 0x F2C */ mVec3_c mTightropeStart;
    /* 0x F38 */ mVec3_c mField_0xF38;
    /* 0x F44 */ mVec3_c mField_0xF44;
    /* 0x F50 */ mVec3_c mField_0xF50;
    /* 0x F5C */ u8 _F5C[0xF68 - 0xF5C];
    /* 0x F68 */ mVec3_c mField_0xF68;
    /* 0x F74 */ u8 _F74[0xFA4 - 0xF74];
    /* 0x FA4 */ f32 mField_0xFA4;
    /* 0x FA8 */ f32 mField_0xFA8;
    /* 0x FAC */ f32 mField_0xFAC;
    /* 0x FB0 */ f32 mField_0xFB0;
    /* 0x FB4 */ f32 mField_0xFB4;
    /* 0x FB8 */ f32 mDistance;
    /* 0x FBC */ f32 mOldDistance;
    /* 0x FC0 */ UNKWORD mField_0xFC0;
    /* 0x FC4 */ f32 mField_0xFC4;
    /* 0x FC8 */ UNKWORD mField_0xFC8;
    /* 0x FCC */ mAng mField_0xFCC;
    /* 0x FCE */ u16 mField_0xFCE;
    /* 0x FD0 */ u8 _FD0[0xFD2 - 0xFD0];
    /* 0x FD2 */ s16 mField_0xFD2;
    /* 0x FD4 */ s16 mField_0xFD4;
    /* 0x FD6 */ s16 mField_0xFD6;
    /* 0x FD8 */ s16 mField_0xFD8;
    /* 0x FDA */ u8 _FD9[0xFE4 - 0xFDA];
    /* 0x FE4 */ u8 mDropsceneFlag;
    /* 0x FE5 */ u8 mWoodTagSceneflag;
    /* 0x FE6 */ u8 mSegmentCount;
    /* 0x FE7 */ u8 mSubtype;
    /* 0x FE8 */ bool mbField_0xFE8;
    /* 0x FE9 */ u8 mField_0xFE9;
    /* 0x FEA */ u8 mField_0xFEA;
    /* 0x FEB */ u8 mField_0xFEB;
    /* 0x FEC */ u8 mField_0xFEC;
    /* 0x FED */ u8 mEventIdx;
    /* 0x FEE */ u8 mField_0xFEE;
    /* 0x FEF */ u8 mField_0xFEF;
    /* 0x FF0 */ u8 mField_0xFF0[4];
    /* 0x FF4 */ u8 mField_0xFF4;
    /* 0x FF5 */ u8 mField_0xFF5;
    /* 0x FF6 */ u8 mField_0xFF6;
    /* 0x FF7 */ u8 mField_0xFF7;
    /* 0x FF8 */ u8 _FE8[0x1005 - 0xFF8];
    /* 0x1005 */ u8 mField_0x1005;
    /* 0x1006 */ u8 _1006[0x1010 - 0x1006];
    /* 0x1010 */ mVec3_c mField_0x1010;
    /* 0x101C */ mVec3_c mField_0x101C;
    /* 0x1028 */ f32 mField_0x1028;
    /* 0x102C */ f32 mField_0x102C;
    /* 0x1030 */ mAng mField_0x1030;
    /* 0x1032 */ mAng mField_0x1032;
    /* 0x1034 */ u8 _1034[0x1038 - 0x1034];
    /* 0x1038 */ u8 mField_0x1038;
    /* 0x1039 */ u8 mField_0x1039; // TODO make field enum
    /* 0x103A */ s8 mField_0x103A;
    /* 0x103B */ u8 mField_0x103B;
    /* 0x103C */ u8 mField_0x103C;
    /* 0x103D */ u8 mField_0x103D;
    /* 0x103E */ bool mDropped;
    /* 0x1040 */ f32 mField_0x1040;
    /* 0x1044 */ u8 _1044[0x105E - 0x1044];
    /* 0x105E */ u8 mField_0x105E;
    /* 0x105F */ u8 mField_0x105F;
    /* 0x1060 */ u8 _1060[0x1088 - 0x1060];
    /* 0x1088 */ dCcD_Cps mCpsArr2[16];
    /* 0x2788 */ mVec3_c *mPnts1;
    /* 0x278C */ mVec3_c *mPnts2;
    /* 0x2790 */ u16 mField_0x2790;
    /* 0x2792 */ u8 mField_0x2792;
    /* 0x2793 */ bool mbField_0x2793;
    /* 0x2794 */ u8 _2794[0x27C0 - 0x2794];
    /* 0x27C0 */ f32 mField_0x27C0;
    /* 0x27C4 */ bool mbField_0x27C4;
    /* 0x27C5 */ bool mbField_0x27C5;

    static InteractionTargetDef sInteraction;
    static dCcD_SrcCps sCpsSrc;
    static dCcD_SrcSph sSphSrc;
    static dCcD_SrcCyl sCylSrc;
};

#endif
