#ifndef D_A_PUMPKIN_H
#define D_A_PUMPKIN_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "d/d_message.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "toBeSorted/d_emitter.h"
#include "nw4r/math/math_triangular.h"
#include "egg/math/eggMath.h"


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
    void fn_19_2C20();
    void fn_19_31B0();
    void fn_19_2FC0(u8 temp);
    void fn_19_30C0();

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcPumpkin_c);
    /* 0x3cc?*/ dBgS_AcchCir* field_0x3cc;
    /* 0x40c?*/ u8 field_0x40c;
    /* spacer*/ u8 spacer[1370];
    /* 0x744?*/ u8 field_0x744;
    /* 0x7ec?*/ u8 field_7ec;
    /* 0x7d8?*/ cCcD_GAtTgCoCommonBase* field_7d8;
    /* 0x79C?*/ u32 pumpkinState;//??
    /* 0x8cc?*/ dFlowBase_c*  something;
    /* 0x870?*/ u32 field_0x870;
    /* 0x908?*/ f32 field_0x908;
    /* 0x90C?*/ f32 field_0x90C;
    /* 0x914?*/ float field0x914[21];
    /* 0x9b8?*/ u32 field0x9b8;
    /* 0x9db?*/ u8 field_0x9db;
    /* 0x99c?*/ u8 field_99c;
    /* 0xa14?*/ u8 field_a14;
    /* 0xa15?*/ u8 field_a15;
    /* 0xa16?*/ u8 field_a16;
    /* 0xa17?*/ u8 field_a17;

};

#endif
