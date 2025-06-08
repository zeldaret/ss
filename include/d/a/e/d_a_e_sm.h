#ifndef D_A_E_SM_H
#define D_A_E_SM_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_jnt_col.h"
#include "d/d_light_env.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/stage_render_stuff.h"
#include "toBeSorted/time_area_mgr.h"


class dAcEsm_c : public dAcEnBase_c {
public:
    enum SmColor_e {
        SM_RED = 0,
        SM_GREEN = 1,
        SM_YELLOW = 2,
        SM_RED_ALT = 3,
        SM_GREEN_ALT = 4,
        SM_YELLOW_ALT = 5,
        SM_BLUE = 6
    };

    enum SmSize_e {
        SM_SMALL = 1,
        SM_LARGE = 2,
        SM_MASSIVE = 3,
        SM_TINY = 4
    };
    struct SmData_c {
        mAng field_0x00;
        mAng field_0x02;
        mVec3_c field_0x04;
        mVec3_c field_0x10;
    };

public:
    dAcEsm_c() : mScnCallback(this), mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEsm_c() {}
    virtual int doDelete();
    virtual int draw();
    virtual bool createHeap();
    virtual int actorCreate();
    virtual int actorPostCreate();
    virtual int actorExecute();
    virtual void registerInEvent();

    STATE_FUNC_DECLARE(dAcEsm_c, BaseMother);
    STATE_FUNC_DECLARE(dAcEsm_c, Wait);
    STATE_FUNC_DECLARE(dAcEsm_c, Walk);
    STATE_FUNC_DECLARE(dAcEsm_c, BirthJump);
    STATE_FUNC_DECLARE(dAcEsm_c, Shake);
    STATE_FUNC_DECLARE(dAcEsm_c, Attack);
    STATE_FUNC_DECLARE(dAcEsm_c, Electrical);
    STATE_FUNC_DECLARE(dAcEsm_c, Absorption);
    STATE_FUNC_DECLARE(dAcEsm_c, Fusion);
    STATE_FUNC_DECLARE(dAcEsm_c, Dead);

    u8 GetParam_s8_0xFF() {
        return params >> 8 & 0xFF;
    }

    u8 GetType() {
        int type = params >> 0 & 0xF;
        return type != 0xF ? type : 0;
    }

    u8 GetParam_s4_0xF() {
        int type = params >> 4 & 0xF;
        return type != 0xF ? type : 0;
    }

    f32 GetParam_s16_0xFF() {
        int tmp = params >> 16 & 0xFF;
        return tmp != 0xFF ? tmp * 100.f : 400.f;
    }

    u8 GetParam_s24_0x7() {
        u8 tmp = params >> 24 & 0x7;
        return tmp != 0x7 ? tmp : 0;
    }

public:
    void fn_187_5D0();
    void fn_187_3F60();
    bool fn_187_4090();
    void fn_187_4200();
    bool fn_187_42C0();
    void fn_187_4450(); // fn_187_4450
    void fn_187_44C0();
    void fn_187_4540(int);
    bool fn_187_4B50();
    bool fn_187_4C50();
    void Set_0xBBC(u8 v);
    void fn_187_4CC0();
    mAng fn_187_5150(bool);
    mAng fn_187_51F0(bool);
    u32 fn_187_52A0();
    void fn_187_5390();
    bool checkSize(SmSize_e) const; // fn_187_5670
    void fn_187_5730();
    void fn_187_5810();
    void fn_187_5940();
    bool fn_187_5AC0() const;
    void fn_187_61B0(u8);
    bool fn_187_6B10();
    void fn_187_6C20(bool);

private:
    void updateBoundingBox();
    void updateMatrix();

    bool *getArray() {
        return sSomeArray;
    }
    void setArray(int i) {
        sSomeArray[i] = false;
    }

private:
    /* 0x378 */ d3d::AnmMdlWrapper mMdl;
    /* 0x3E8 */ dShadowCircle_c mShadowCircle;
    /* 0x3F0 */ m3d::anmTexPat_c mAnmTexPat;
    /* 0x41C */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x448 */ dScnCallback_c mScnCallback;
    /* 0x454 */ dCcD_Sph mSph;
    /* 0x5A4 */ dBgS_AcchCir mAcchCir;
    /* 0x600 */ dBgS_ObjAcch mObjAcch;
    /* 0x9B0 */ STATE_MGR_DECLARE(dAcEsm_c);
    /* 0x9EC */ dAcRef_c<dAcBomb_c> mBombRef;
    /* 0x9F8 */ dJntCol_c mJntCol;
    /* 0xA0C */ EffectsStruct mEffArr[2];
    /* 0xA74 */ TimeAreaStruct mTimeArea;
    /* 0xA80 */ mVec3_c mPosCopy1;
    /* 0xA8C */ mVec3_c mScaleTarget;
    /* 0xA98 */ mVec3_c mScaleCopy2;
    /* 0xAA4 */ mVec3_c mHitPos;
    /* 0xAB0 */ u8 _AB0[0xAC8 - 0xAB0];
    /* 0xAC8 */ mVec3_c mHomePos1;
    /* 0xAD4 */ mVec3_c mEffPos;
    /* 0xAE0 */ dWaterEffect_c mSplashFx;
    /* 0xB28 */ mAng3_c mRotUnk;
    /* 0xB2E */ mAng mTargetRotX;
    /* 0xB30 */ mAng mTargetRotZ;
    /* 0xB32 */ u8 _B32[0xB38 - 0xB32];
    /* 0xB38 */ mAng3_c mRotCopy;
    /* 0xB3E */ mAng mOrigRotZ;
    /* 0xB40 */ f32 field_0xB40;
    /* 0xB44 */ f32 field_0xB44;
    /* 0xB48 */ f32 field_0xB48;
    /* 0xB4C */ f32 field_0xB4C;
    /* 0xB50 */ f32 field_0xB50;
    /* 0xB54 */ f32 field_0xB54;
    /* 0xB58 */ f32 field_0xB58;
    /* 0xB5C */ f32 field_0xB5C;
    /* 0xB60 */ u8 _B60[0xB65 - 0xB60];
    /* 0xB65 */ u8 field_0xB65;
    /* 0xB66 */ u8 _B66[0xB68 - 0xB66];
    /* 0xB68 */ f32 field_0xB68;
    /* 0xB6C */ f32 field_0xB6C;
    /* 0xB70 */ f32 field_0xB70;
    /* 0xB74 */ f32 field_0xB74;
    /* 0xB78 */ f32 field_0xB78;
    /* 0xB7C */ u32 field_0xB7C;
    /* 0xB80 */ f32 field_0xB80;
    /* 0xB84 */ f32 field_0xB84;
    /* 0xB88 */ u32 field_0xB88;
    /* 0xB8C */ f32 field_0xB8C;
    /* 0xB90 */ s32 field_0xB90;
    /* 0xB94 */ s32 field_0xB94;
    /* 0xB98 */ s32 field_0xB98;
    /* 0xB9C */ u8 _B9C[0xBA0 - 0xB9C];
    /* 0xBA0 */ u32 field_0xBA0;
    /* 0xBA4 */ s16 field_0xBA4;
    /* 0xBA6 */ s16 field_0xBA6;
    /* 0xBA8 */ u16 field_0xBA8;
    /* 0xBAA */ u16 field_0xBAA;
    /* 0xBAC */ u16 field_0xBAC;
    /* 0xBAE */ u16 mTimer_0xBAE;
    /* 0xBB0 */ u16 field_0xBB0;
    /* 0xBB2 */ u16 field_0xBB2;
    /* 0xBB4 */ u16 mDamageTimer;
    /* 0xBB6 */ u16 field_0xBB6;
    /* 0xBB8 */ u16 field_0xBB8;
    /* 0xBB9 */ u8 _BB9[0xBBC - 0xBBA];
    /* 0xBBC */ u8 field_0xBBC;
    /* 0xBBD */ u8 field_0xBBD;
    /* 0xBBE */ u8 mType;
    /* 0xBBF */ u8 field_0xBBF;
    /* 0xBC0 */ u8 field_0xBC0;
    /* 0xBC1 */ u8 field_0xBC1;
    /* 0xBC2 */ u8 field_0xBC2;
    /* 0xBC3 */ u8 shift8_0xFF;
    /* 0xBC4 */ u8 mTimer_0xBC4;
    /* 0xBC5 */ u8 field_0xBC5;
    /* 0xBC6 */ u8 field_0xBC6;
    /* 0xBC7 */ u8 field_0xBC7;
    /* 0xBC8 */ u8 field_0xBC8;
    /* 0xBC9 */ u8 field_0xBC9;
    /* 0xBCA */ u8 field_0xBCA;
    /* 0xBCB */ u8 field_0xBCB;
    /* 0xBCC */ u8 field_0xBCC;
    /* 0xBCD */ u8 field_0xBCD;
    /* 0xBCE */ u8 field_0xBCE;
    /* 0xBD0 */ LIGHT_INFLUENCE mLightInfo;

    static bool sSomeArrayInit;
    static bool sSomeArray[9];

    static const u16 sEmitterResArr[8];
    static const SmData_c sSmDataArr[8];

    static s32 sValueFromPlayer;
    static s32 sTimer;
};

#endif
