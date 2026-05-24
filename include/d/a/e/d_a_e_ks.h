#ifndef D_A_E_KS_H
#define D_A_E_KS_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_light_env.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_path.h"
#include "toBeSorted/time_area_mgr.h"

class dTgKiesuTag_c;
class dAcEKs_c : public dAcEnBase_c {
public:
    dAcEKs_c() : mStateMgr(*this) {}
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

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcEKs_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcEKs_c);

    enum Type_e {
        EKS_NORMAL,
        EKS_FIRE,
        EKS_ELECTRIC,
        EKS_CURSED,
    };
    enum StartingStateParam_e {
        EKS_STARTSTATE_Move = 0,
        EKS_STARTSTATE_Wait = 1,
        EKS_STARTSTATE_PathMove = 2,
    };
    enum State_e {
        EKS_STATE_Move = 0,
        EKS_STATE_Wait = 1,
        EKS_STATE_PathMove = 2,
        EKS_STATE_WakeUp = 4,
        EKS_STATE_ReturnToWait = 5,
        EKS_STATE_Chase = 6,
        EKS_STATE_ChaseAttack = 7,
        EKS_STATE_Fighting = 8,
        EKS_STATE_AttackReady = 9,
        EKS_STATE_Attack = 10,
        EKS_STATE_Damage = 11,
        EKS_STATE_Stun = 12,
        EKS_STATE_WindBlow = 13,
    };
    enum AnmTexPat_e {
        EKS_TEXPAT_BLINK1,
        EKS_TEXPAT_BLINK2,
        EKS_TEXPAT_BLINK3,
    };

public:
    void linkKiesuTag(dTgKiesuTag_c *pTgKs);
    void setStartingState();
    void setIdleState();

    void fn_155_A60();

    f32 getLineCrossYRange(const mVec3_c &pos, f32 range);

    void kill(bool dropItem);
    void chaseTargetY(f32, f32);
    void playBlinkAnm();
    void soundSqueak();
    void soundStun();
    void fn_155_1430();

    void fn_155_1470();

    // Move impl?
    void fn_155_1C80();

    /* 0 -> blink_1, 1 -> blink_2, 2 -> blink_3*/
    void setBlink(u8 blink);
    void setBlinkChecked(u8 blink);

    void fn_155_2270();
    /* p1 = 0 -> just LineCross return check,  1 ->  increment field_0xDCB till 10 then return check*/
    bool fn_155_29D0(s32 p1);
    bool fn_155_2B80(bool b, s32 p1);
    bool fn_155_2D60(bool b, s32 p1);
    bool fn_155_2EA0();
    bool isOutsideRange();
    bool isNotWaitingNorDamage();
    bool isTargeted();
    bool checkWallCross();
    void adjustTargetGnd();
    void adjustTargetRoof();
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
    void clampRotationX();
    bool checkPathPntParam(u32 param);
    void fn_155_3E30();
    void fn_155_3E90();
    void fn_155_3EC0();

    /* Some Timestone Check */
    bool fn_155_3EF0();

    void fn_155_3F50();

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
    /* 0xAA8 */ TimeAreaStruct mTimeArea;
    /* 0xAB4 */ mMtx_c mCenterWorldMtx;
    /* 0xAE4 */ mMtx_c field_0xAE4;
    /* 0xB14 */ mVec3_c mPnts[10];
    /* 0xB8C */ mVec3_c field_0xB8C;
    /* 0xB98 */ mVec3_c field_0xB98;
    /* 0xBA4 */ mVec3_c field_0xBA4;
    /* 0xBB0 */ mVec3_c mTargetPos;
    /* 0xBBC */ mVec3_c field_0xBBC;
    /* 0xBC8 */ mVec3_c field_0xBC8;
    /* 0xBD4 */ mVec3_c field_0xBD4;
    /* 0xBE0 */ mVec3_c field_0xBE0;
    /* 0xBEC */ mVec3_c field_0xBEC;
    /* 0xBF8 */ mVec3_c field_0xBF8;
    /* 0xC04 */ mVec3_c field_0xC04;
    /* 0xC10 */ mVec3_c field_0xC10;
    /* 0xC1C */ mVec3_c field_0xC1C;
    /* 0xC28 */ mVec3_c field_0xC28;
    /* 0xC34 */ mAng mPitch_0xC34;
    /* 0xC36 */ mAng mYaw_0xC36;
    /* 0xC38 */ u8 _0xC38[0xC3C - 0xC38];
    /* 0xC3C */ f32 field_0xC3C;
    /* 0xC40 */ f32 field_0xC40;
    /* 0xC44 */ f32 field_0xC44;
    /* 0xC48 */ f32 field_0xC48;
    /* 0xC4C */ u8 _0xC4C[0xC70 - 0xC4C];
    /* 0xC70 */ f32 field_0xC70;
    /* 0xC74 */ u8 _0xC74[0xC90 - 0xC74];
    /* 0xC90 */ f32 field_0xC90;
    /* 0xC94 */ u8 _0xC94[0xCE0 - 0xC94];
    /* 0xCE0 */ f32 field_0xCE0;
    /* 0xCE4 */ f32 field_0xCE4;
    /* 0xCE8 */ u8 _0xCE8[0xCF8 - 0xCE8];
    /* 0xCF8 */ f32 field_0xCF8;
    /* 0xCFC */ f32 field_0xCFC;
    /* 0xD00 */ f32 field_0xD00;
    /* 0xD04 */ f32 field_0xD04;
    /* 0xD08 */ f32 field_0xD08;
    /* 0xD0C */ f32 field_0xD0C;
    /* 0xD10 */ f32 field_0xD10;
    /* 0xD14 */ f32 field_0xD14;
    /* 0xD18 */ f32 field_0xD18;
    /* 0xD1C */ f32 field_0xD1C;
    /* 0xD20 */ f32 field_0xD20;
    /* 0xD24 */ f32 field_0xD24;
    /* 0xD28 */ f32 field_0xD28;
    /* 0xD2C */ u32 mCenterNode;
    /* 0xD30 */ u8 field_0xD30;
    /* 0xD31 */ u8 field_0xD31;
    /* 0xD32 */ u16 mTimer;
    /* 0xD34 */ u8 _0xD34[0xD4A - 0xD34];
    /* 0xD4A */ s16 field_0xD4A;
    /* 0xD4C */ s16 field_0xD4C;
    /* 0xD4E */ s16 field_0xD4E;
    /* 0xD50 */ s16 field_0xD50;
    /* 0xD52 */ s16 field_0xD52;
    /* 0xD54 */ s16 field_0xD54;
    /* 0xD56 */ s16 field_0xD56;
    /* 0xD58 */ s16 field_0xD58;
    /* 0xD5A */ s16 field_0xD5A;
    /* 0xD5C */ s16 field_0xD5C;
    /* 0xD5E */ s16 field_0xD5E;
    /* 0xD60 */ s16 field_0xD60;
    /* 0xD62 */ s16 field_0xD62;
    /* 0xD64 */ s16 field_0xD64;
    /* 0xD66 */ s16 field_0xD66;
    /* 0xD68 */ s16 field_0xD68;
    /* 0xD6A */ s16 field_0xD6A;
    /* 0xD6C */ s16 field_0xD6C;
    /* 0xD6E */ s16 field_0xD6E;
    /* 0xD70 */ s16 field_0xD70;
    /* 0xD72 */ s16 field_0xD72;
    /* 0xD74 */ s16 field_0xD74;
    /* 0xD76 */ s16 field_0xD76;
    /* 0xD78 */ s16 mBlinkTimer;
    /* 0xD7A */ u16 field_0xD7A;
    /* 0xD7C */ s16 field_0xD7C;
    /* 0xD7E */ s16 mSqueakSoundTimer;
    /* 0xD80 */ s16 mStunSoundTimer;
    /* 0xD82 */ s16 field_0xD82;
    /* 0xD84 */ s16 field_0xD84;
    /* 0xD86 */ s16 field_0xD86;
    /* 0xD88 */ s16 field_0xD88;
    /* 0xD8A */ s16 field_0xD8A;
    /* 0xD8C */ s16 field_0xD8C;
    /* 0xD8E */ u16 field_0xD8E;
    /* 0xD90 */ s16 field_0xD90;
    /* 0xD92 */ s16 field_0xD92;
    /* 0xD94 */ s16 field_0xD94;
    /* 0xD96 */ s16 field_0xD96;
    /* 0xD98 */ s16 field_0xD98;
    /* 0xD9A */ s16 field_0xD9A;
    /* 0xD9C */ s16 field_0xD9C;
    /* 0xD9E */ s16 field_0xD9E;
    /* 0xDA0 */ s16 field_0xDA0;
    /* 0xDA2 */ s16 field_0xDA2;
    /* 0xDA4 */ u8 field_0xDA4;
    /* 0xDA5 */ u8 field_0xDA5;
    /* 0xDA6 */ u8 field_0xDA6;
    /* 0xDA7 */ u8 field_0xDA7;
    /* 0xDA8 */ u8 field_0xDA8;
    /* 0xDA9 */ u8 field_0xDA9;
    /* 0xDAA */ u8 field_0xDAA;
    /* 0xDAB */ u8 field_0xDAB;
    /* 0xDAC */ u8 field_0xDAC;
    /* 0xDAD */ u8 field_0xDAD;
    /* 0xDAE */ u8 field_0xDAE;
    /* 0xDAF */ u8 field_0xDAF;
    /* 0xDB0 */ u8 field_0xDB0;
    /* 0xDB1 */ u8 field_0xDB1;
    /* 0xDB2 */ u8 field_0xDB2;
    /* 0xDB3 */ u8 field_0xDB3;
    /* 0xDB4 */ u8 field_0xDB4;
    /* 0xDB5 */ u8 field_0xDB5;
    /* 0xDB6 */ u8 field_0xDB6;
    /* 0xDB7 */ u8 field_0xDB7;
    /* 0xDB8 */ u8 field_0xDB8;
    /* 0xDB9 */ u8 field_0xDB9;
    /* 0xDBA */ u8 field_0xDBA;
    /* 0xDBB */ u8 field_0xDBB;
    /* 0xDBC */ u8 mType;
    /* 0xDBD */ u8 field_0xDBD;
    /* 0xDBE */ u8 field_0xDBE;
    /* 0xDBF */ u8 field_0xDBF;
    /* 0xDC0 */ u8 mCurrentAnmTexPat; // blink
    /* 0xDC1 */ u8 field_0xDC1;
    /* 0xDC2 */ u8 mCurrentState;
    /* 0xDC3 */ u8 mNextState;
    /* 0xDC4 */ u8 field_0xDC4;
    /* 0xDC5 */ u8 field_0xDC5;
    /* 0xDC6 */ u8 field_0xDC6;
    /* 0xDC7 */ u8 mStartingState;
    /* 0xDC8 */ u8 field_0xDC8;
    /* 0xDC9 */ u8 field_0xDC9;
    /* 0xDCA */ u8 field_0xDCA;
    /* 0xDCB */ u8 field_0xDCB;
    /* 0xDCC */ u8 field_0xDCC;
    /* 0xDCD */ u8 field_0xDCD;
    /* 0xDCE */ u8 field_0xDCE;
    /* 0xDCF */ u8 field_0xDCF;
    /* 0xDD0 */ u8 field_0xDD0;
    /* 0xDD4 */ LIGHT_INFLUENCE mLightInfluence;

    // Controlled by dTgKiesuTag_c
    static bool lbl_155_bss_388;
    static bool lbl_155_bss_389;
};

#endif
