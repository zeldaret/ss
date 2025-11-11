#ifndef D_A_OBJ_BOMB_H
#define D_A_OBJ_BOMB_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "toBeSorted/d_emitter.h"

// This may need its own file and could be independent of a bomb
class UnkBombColInfo : public cBgS_PolyInfo {
public:
    UnkBombColInfo() : field_0x10(0), field_0x14(0), field_0x18(0) {}
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

class dAcBomb_c : public dAcObjBase_c {
public:
    dAcBomb_c() : mStateMgr(*this, sStateID::null), mSelfRef(this), mEffect1(this), mEffect2(this) {}
    virtual ~dAcBomb_c();

    /* vt 0x08  */ virtual int create() override;
    /* vt 0x14  */ virtual int doDelete() override;
    /* vt 0x2C  */ virtual int draw() override;
    /* vt 0x44  */ virtual bool createHeap() override;
    /* vt 0x54  */ virtual int actorExecute() override;
    /* vt 0x5C  */ virtual void unkVirtFunc_0x5C() override;
    /* vt 0x68  */ virtual void registerInEvent() override;
    /* vt 0x6C  */ virtual void unkVirtFunc_0x6C() override;

    STATE_FUNC_DECLARE(dAcBomb_c, Wait);
    STATE_FUNC_DECLARE(dAcBomb_c, FlowerWait);
    STATE_FUNC_DECLARE(dAcBomb_c, Explode);
    STATE_FUNC_DECLARE(dAcBomb_c, Carry);
    STATE_FUNC_DECLARE(dAcBomb_c, WindCarry);

    void setTransformFromFlower(const mMtx_c &);

    void unsetField0xA3C(u32 mask) {
        field_0xA3C &= ~mask;
    }
    void setField0xA3C(u32 mask) {
        field_0xA3C |= mask;
    }

public:
    /* 0x330 */ nw4r::g3d::ResFile mBrres;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShdw;
    /* 0x358 */ dBgS_BombAcch mAcch;
    /* 0x708 */ dBgS_AcchCir mAcchCir;
    /* 0x764 */ dCcD_Sph mCcDSph;
    /* 0x8B4 */ mMtx_c mMtx;
    /* 0x8E4 */ dBgS_BombLinChk mLinChk;
    /* 0x97C */ UnkBombColInfo mUnkInfo;
    /* 0x9B4 */ u8 _9B4[0x9B8 - 0x9B4];
    /* 0x9B8 */ dAcBomb_c *mSelfRef;
    /* 0x9BC */ u8 _9BC[0x9D4 - 0x9BC];
    /* 0x9D4 */ dEmitter_c mEffect1;
    /* 0xA08 */ dEmitter_c mEffect2;
    /* 0xA3C */ u32 field_0xA3C;
    /* 0xA40 */ u32 field_0xA40;
    /* 0xA44 */ s16 field_0xA44;
    /* 0xA46 */ u8 _3[0xA50 - 0xA46];
    /* 0xA50 */ f32 field_0xA50;
    /* 0xA54 */ u8 _4[0xA9C - 0xA54];
    /* 0xA9C */ STATE_MGR_DECLARE(dAcBomb_c);
};

#endif
