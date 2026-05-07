#ifndef D_A_E_KS_H
#define D_A_E_KS_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_path.h"

class dTgKiesuTag_c;
class dAcEKs_c : public dAcEnBase_c {
public:
    dAcEKs_c() : mStateMgr(*this), field_0xAA8(0.f), field_0xAB0(0) {}
    virtual ~dAcEKs_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual bool restorePosRotFromCopy() override;

    STATE_FUNC_DECLARE(dAcEKs_c, Wait);
    STATE_FUNC_DECLARE(dAcEKs_c, WakeUp);
    STATE_FUNC_DECLARE(dAcEKs_c, ReturnToWait);
    STATE_FUNC_DECLARE(dAcEKs_c, WaitReady);
    STATE_FUNC_DECLARE(dAcEKs_c, Move);
    STATE_FUNC_DECLARE(dAcEKs_c, Chase);
    STATE_FUNC_DECLARE(dAcEKs_c, ChaseAttack);
    STATE_FUNC_DECLARE(dAcEKs_c, Fighting);
    STATE_FUNC_DECLARE(dAcEKs_c, AttackReady);
    STATE_FUNC_DECLARE(dAcEKs_c, Attack);
    STATE_FUNC_DECLARE(dAcEKs_c, Damage);
    STATE_FUNC_DECLARE(dAcEKs_c, Stun);
    STATE_FUNC_DECLARE(dAcEKs_c, WindBlow);
    STATE_FUNC_DECLARE(dAcEKs_c, PathMove);

public:
    void linkKiesuTag(dTgKiesuTag_c *pTgKs);
    void setStartingState();
    void setIdleState();

    f32 getLineCrossYRange(const mVec3_c &pos, f32 range);

    /* 0 -> blink_1, 1 -> blink_2, 2 -> blink_3*/
    void setBlink(u8 blink);
    void setBlinkChecked(u8 blink);
    bool fn_155_29D0(s32 p1);
    bool fn_155_2D60(bool b, s32 p1);
    bool fn_155_2EA0();
    bool isOutsideRange();
    bool isNotWaitingNorDamage();
    bool isTargeted();
    bool ChkWall();
    void ChkGnd();
    void ChkRoof();
    bool ChkHit();
    void fn_155_3460();
    void fn_155_3480();
    bool transitionToNextState();
    void setAnim(char *, f32, f32, f32);
    void fn_155_3720(f32, f32);
    void fn_155_3750(u8 state);
    void fn_155_37A0();
    void fn_155_37F0();
    void fn_155_3840();
    void fn_155_3890();
    void on_lbl_155_bss_389();
    /* 0: Checks states to set AC_PROP_0x1 - 1: Unset AC_PROP_0x1*/
    void fn_155_3900(s32 p0);
    void setPitchYawToPoint(const mVec3_c &pnt);
    void fn_155_3BD0(const mVec3_c &);
    void fn_155_3BE0(const mVec3_c &);
    void fn_155_3E90();
    void fn_155_3EC0();

private:
    /* 0x378 */ dAcRef_c<dTgKiesuTag_c> mTgRef;
    /* 0x384 */ nw4r::g3d::ResFile mResFile;
    /* 0x388 */ d3d::AnmMdlWrapper mMdl;
    /* 0x3F8 */ m3d::anmTexPat_c mAnmTexPat;
    /* 0x324 */ dShadowCircle_c mShadow;
    /* 0x42C */ dBgS_AcchCir mAcchCir;
    /* 0x488 */ dBgS_ObjAcch mAcch;
    /* 0x838 */ dCcD_Sph mSph;
    /* 0x988 */ STATE_MGR_DECLARE(dAcEKs_c);
    /* 0x9C4 */ dEmitter_c mEmitter1;
    /* 0x9F8 */ dEmitter_c mEmitter2;
    /* 0xA2C */ dWaterEffect_c mWaterEmitter;
    /* 0xA74 */ ActorOnRail_Ext mRail;
    /* 0xAA8 */ f32 field_0xAA8;
    /* 0xAAC */ UNKWORD field_0xAAC;
    /* 0xAA8 */ u8 field_0xAB0;
    /* 0xAB1 */ u8 _0xAB1[0xB14 - 0xAB1];
    /* 0xB14 */ mVec3_c mPnts[10];
    /* 0xB8C */ u8 _0xB8C[0xBB0 - 0xB8C];
    /* 0xBB0 */ mVec3_c field_0xBB0;
    /* 0xBBC */ mVec3_c field_0xBBC;
    /* 0xBC8 */ u8 _0xBC8[0xBF8 - 0xBC8];
    /* 0xBF8 */ mVec3_c field_0xBF8;
    /* 0xC04 */ u8 _0xC04[0xC28 - 0xC04];
    /* 0xC28 */ mVec3_c field_0xC28;
    /* 0xC34 */ mAng mPitch_0xC34;
    /* 0xC36 */ mAng mYaw_0xC36;
    /* 0xC38 */ u8 _0xC38[0xC3C - 0xC38];
    /* 0xC3C */ f32 field_0xC3C;
    /* 0xC40 */ f32 field_0xC40;
    /* 0xC44 */ u8 _0xC44[0xD1C - 0xC44];
    /* 0xD1C */ f32 field_0xD1C;
    /* 0xD20 */ f32 field_0xD20;
    /* 0xD24 */ u8 _0xD24[0xD32 - 0xD24];
    /* 0xD32 */ s16 mTimer;
    /* 0xD34 */ u8 _0xD34[0xD60 - 0xD34];
    /* 0xD60 */ s16 field_0xD60;
    /* 0xD62 */ u8 _0xD62[0xDA6 - 0xD62];
    /* 0xDA6 */ u8 field_0xDA6;
    /* 0xDA7 */ u8 field_0xDA7;
    /* 0xDA8 */ u8 field_0xDA8;
    /* 0xDA9 */ u8 field_0xDA9;
    /* 0xDAA */ u8 _0xDAA[0xDB1 - 0xDAA];
    /* 0xDB1 */ u8 field_0xDB1;
    /* 0xDB2 */ u8 field_0xDB2;
    /* 0xDB3 */ u8 _0xDB3[0xDBB - 0xDB3];
    /* 0xDBB */ u8 field_0xDBB;
    /* 0xDBC */ u8 mType;
    /* 0xDBD */ u8 _0xDBD[0xDC0 - 0xDBD];
    /* 0xDC2 */ u8 mCurrentAnmTexPat; // blink
    /* 0xDC1 */ u8 field_0xDC1;
    /* 0xDC2 */ u8 mCurrentState;
    /* 0xDC3 */ u8 mNextState;
    /* 0xDC4 */ u8 _0xDC4[0xDC7 - 0xDC4];
    /* 0xDC7 */ u8 mStartingState;
    /* 0xDC8 */ u8 _0xDC8[0xDCF - 0xDB8];
    /* 0xDCF */ u8 field_0xDCF;
    /* 0xDD0 */ u8 _0xDD0[0xDD4 - 0xDD0];
    /* 0xDD4 */ mVec3_c mHomePos;
    /* 0xDE0 */ mColor mColor;
    /* 0xDE4 */ f32 field_0xDE4;
    /* 0xDE8 */ u8 _0xDE8[0xDF0 - 0xDE8];

    // Controlled by dTgKiesuTag_c
    static bool lbl_155_bss_388;
    static bool lbl_155_bss_389;
};

#endif
