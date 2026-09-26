#ifndef D_A_PUMPKIN_H
#define D_A_PUMPKIN_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "d/d_message.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "toBeSorted/d_emitter.h"
#include "nw4r/math/math_triangular.h"
#include "egg/math/eggMath.h"
#include "d/d_shadow.h"


class dAcPumpkin_c : public dAcObjBase_c {
public:
    dAcPumpkin_c() : mStateMgr(*this) {}
    //virtual ~dAcPumpskin_c() {}??

    STATE_FUNC_DECLARE(dAcPumpkin_c, Stick);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Wait);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryGrab);
    STATE_FUNC_DECLARE(dAcPumpkin_c, StickSword);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Throw);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Roll);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryBeetle);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryWhip);
    void fn_19_220();
    void fn_19_2C20();//destroy?
    void fn_19_31B0();
    void fn_19_2FC0(u8 temp);
    void fn_19_30C0();
    void fn_19_2FD0();
    void fn_19_4a0();
    void fn_19_500(u32* unused1, u32* unused2, u32* tempPtr);
    int fn_19_520(u32* param1, u32* param2, u32* param3);
    bool fn_19_550();
    int fn_19_940();
    int fn_19_950();
    void fn_19_B80();
    void fn_19_2920();
    bool fn_19_2E10();
    bool fn_19_2F60();
    static void fn_19_3130(mVec3_c* param1, dAcPumpkin_c* param2);
    void fn_19_31b0();
    void fn_19_2900();

    bool angleToRotate(const mVec3_c& vec);//temp

private:
    /* 0x330*/ m3d::smdl_c field_0x330;
    /* 0x34C*/ m3d::smdl_c field_0x34C;
    /* 0x368*/ dShadowCircle_c mShdw;
    /* 0x370*/ dBgS_AcchCir mAcchCir;
    /* 0x3CC*/ dBgS_ObjAcch mObjAcch;
    /* 0x77C */ dCcD_Sph mSph;
    /* 0x8cc */ STATE_MGR_DECLARE(dAcPumpkin_c);
    /* 0x908?*/ f32 field_0x908;//distance traveled? also used to make a put sound, so maybe falling distance?
    //everything above should be offset correctly.
    /* 0x90C?*/ mAng field_0x90C;
    /* 0x90E?*/ mAng field_0x90E;
    /* 0x912?*/ s16 field_0x912;
    /* 0x914?*/ float field_0x914[21];
    /* 0x964?*/ s32 field_0x964;
    /* 0x968?*/ mMtx_c field_0x968;
    /* 0x998?*/ s32 field_0x998;
    /* 0x99c?*/ u8 field_0x99c;
    /* 0x99e?*/ u16 field_0x99e;
    /* 0x9a0?*/ s16 field_0x9a0;
    /* 0x9a4?*/ f32 field_0x9a4;
    /* 0x9a8?*/ f32 field_0x9a8;
    /* 0x9b4?*/ s16 field_0x9b4;
    /* 0x9b6?*/ u8 field_0x9b6;
    /* 0x9b8?*/ s16 field_0x9b8;
    /* 0x9bc?*/ u8 field_0x9bc;
    /* 0x9c0?*/ mQuat_c field_0x9c0;
    /* 0x9d0?*/ u8 field_0x9d0;
    /* 0x9d4?*/ //fLiNdBa_c field_0x9D4;
    /* 0x9db?*/ u8 field_0x9db;
    /* 0x9dc?*/ u32 field_0x9dc;
    /* 0x9e0?*/ dEmitter_c field_0x9E0;
    /* 0xa14?*/ u8 field_0xa14;
    /* 0xa15?*/ u8 field_0xa15;//something to do with water
    /* 0xa16?*/ u8 field_0xA16;
    /* 0xa17?*/ u8 field_0xa17;
    /* 0xa18?*/ dWaterEffect_c field_0xa18;
    /* 0x? */ //dShadowCircle_c mShdw;

};

#endif
