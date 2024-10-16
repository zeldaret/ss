#ifndef D_A_BOMB_H
#define D_A_BOMB_H

#include "d/a/obj/d_a_obj_base.h"

#include <d/col/bg/d_bg_s_acch.h>
#include <d/col/bg/d_bg_s_lin_chk.h>
#include <d/col/cc/d_cc_shape_colliders.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/effects_struct.h>

class dAcBomb_c : public dAcObjBase_c {
public:
    dAcBomb_c();
    virtual ~dAcBomb_c();

    /* vt 0x08  */ virtual int create() override;
    /* vt 0x14  */ virtual int doDelete() override;
    /* vt 0x2C  */ virtual int draw() override;
    /* vt 0x44  */ virtual bool createHeap() override;
    /* vt 0x54  */ virtual int actorExecute() override;
    /* vt 0x5C  */ virtual void unkVirtFunc_0x5C() override;
    /* vt 0x68  */ virtual void *getCurrentEventActor() override;
    /* vt 0x6C  */ virtual void unkVirtFunc_0x6C() override;

    STATE_FUNC_DECLARE(dAcBomb_c, Wait);
    STATE_FUNC_DECLARE(dAcBomb_c, FlowerWait);
    STATE_FUNC_DECLARE(dAcBomb_c, Explode);
    STATE_FUNC_DECLARE(dAcBomb_c, Carry);
    STATE_FUNC_DECLARE(dAcBomb_c, WindCarry);

private:
    /* 0x330 */ nw4r::g3d::ResFile mBrres;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ u8 _0[0x358 - 0x350];
    /* 0x358 */ dBgS_BombAcch mAcch;
    /* 0x708 */ dBgS_AcchCir mAcchCir;
    /* 0x764 */ dCcD_Sph mCcDSph;
    /* 0x8B4 */ mMtx_c mMtx;
    /* 0x8E4 */ dBgS_BombLinChk mLinChk;
    /* 0x97C */ u8 _1[0x9D4 - 0x97C];
    /* 0x9D4 */ EffectsStruct mEffect1;
    /* 0xA08 */ EffectsStruct mEffect2;
    /* 0xA3C */ u8 _2[0xA9C - 0xA3C];
    /* 0xA9C */ STATE_MGR_DECLARE(dAcBomb_c);
};

#endif
