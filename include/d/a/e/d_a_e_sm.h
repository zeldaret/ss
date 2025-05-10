#ifndef D_A_E_SM_H
#define D_A_E_SM_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/blur_and_palette_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcEsm_c : public dAcEnBase_c {
public:
    enum SmColor_e {
        SM_RED,
        SM_GREEN,
        SM_YELLOW,
        SM_RED_ALT,
        SM_GREEN_ALT,
        SM_YELLOW_ALT,
        SM_BLUE
    };

    enum SmSize_e {
        SM_SMALL = 1,
        SM_LARGE = 2,
        SM_MASSIVE = 3,
        SM_TINY = 4
    };

public:
    dAcEsm_c() : mScnCallback(this), mStateMgr(*this, sStateID::null), field_0xA74(0.f), field_0xA7C(0) {}
    virtual ~dAcEsm_c() {}
    virtual int doDelete();
    virtual int draw();
    virtual bool createHeap();
    virtual int actorCreate();
    virtual int actorPostCreate();
    virtual int actorExecute();
    virtual void registerInEvent();
    virtual void *getObjectListEntry();

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
    bool fn_187_4B50();
    UNKTYPE fn_187_4CB0(u8);
    UNKTYPE fn_187_6C20(u8);
    UNKTYPE fn_187_44C0();
    bool checkSize(SmSize_e) const; // fn_187_5670

private:
    void updateBoundingBox();
    void updateMatrix();

    static int sDefaultRotX;
    static int sDefaultRotY;
    static int sDefaultRotZ;
    static dCcD_SrcSph sSphSrc;

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
    /* 0x9F8 */ todoStruct00 field_0x9f8;
    /* 0xA0C */ EffectsStruct mEffArr[2];
    /* 0xA74 */ f32 field_0xA74;
    /* 0xA78 */ u8 _0xA78[4];
    /* 0xA7C */ u8 field_0xA7C;
    /* 0xA7D */ u8 _A7D[0xA80 - 0xA7D];
    /* 0xA80 */ mVec3_c mPosCopy1;
    /* 0xA8C */ mVec3_c mScaleCopy1;
    /* 0xA98 */ mVec3_c mScaleCopy2;
    /* 0xAA4 */ u8 _AA4[0xAC8 - 0xAA4];
    /* 0xAC8 */ mVec3_c mHomePos1;
    /* 0xAD4 */ mVec3_c mHomePos2;
    /* 0xAE0 */ dWaterEffect_c mEffExt;
    /* 0xB28 */ mAng3_c mRotUnk;
    /* 0xB2E */ u8 _B2E[0xB38 - 0xB2E];
    /* 0xB38 */ mAng3_c mRotCopy;
    /* 0xB3E */ mAng mOrigRotZ;
    /* 0xB40 */ u8 _B40[0xB58 - 0xB40];
    /* 0xB58 */ f32 field_0xB58;
    /* 0xB5C */ u8 _B5C[0xB65 - 0xB5C];
    /* 0xB65 */ u8 field_0xB65;
    /* 0xB66 */ u8 _B66[0xB6C - 0xB66];
    /* 0xB6C */ f32 field_0xB6C;
    /* 0xB70 */ f32 field_0xB70;
    /* 0xB74 */ f32 field_0xB74;
    /* 0xB78 */ f32 field_0xB78;
    /* 0xB7C */ u32 field_0xB7C;
    /* 0xB80 */ u32 field_0xB80;
    /* 0xB84 */ u32 field_0xB84;
    /* 0xB88 */ u32 field_0xB88;
    /* 0xB8C */ f32 field_0xB8C;
    /* 0xB90 */ s32 field_0xB90;
    /* 0xB94 */ u8 _B94[0xB98 - 0xB94];
    /* 0xB98 */ u32 field_0xB98;
    /* 0xB9C */ u8 _B9C[0xBA6 - 0xB9C];
    /* 0xBA6 */ s16 field_0xBA6;
    /* 0xBA8 */ u8 _BA8[0xBAE - 0xBA8];
    /* 0xBAE */ s16 timer_0xBAE;
    /* 0xBAC */ u8 _BAC[0xBBE - 0xBAC];
    /* 0xBBE */ u8 mType;
    /* 0xBBF */ u8 field_0xBBF;
    /* 0xBC0 */ u8 _BC0[0xBC3 - 0xBC0];
    /* 0xBC3 */ u8 shift8_0xFF;
    /* 0xBC4 */ u8 field_0xBC4;
    /* 0xBC5 */ u8 field_0xBC5;
    /* 0xBC6 */ u8 field_0xBC6;
    /* 0xBC7 */ u8 field_0xBC7;
    /* 0xBC8 */ u8 field_0xBC8;
    /* 0xBC9 */ u8 field_0xBC9;
    /* 0xBCA */ u8 field_0xBCA;
    /* 0xBCB */ u8 field_0xBCB;
    /* 0xBCC */ u8 field_0xBCC;
    /* 0xBCD */ u8 _BCD[0xBD0 - 0xBCD];
    /* 0xBD0 */ LightParams mLightInfo;

    static bool sSomeArrayInit;
    static bool sSomeArray[9];
};

#endif
