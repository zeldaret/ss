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
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcBomb_c : public dAcObjBase_c {
public:
    dAcBomb_c() : mStateMgr(*this), mUnkRef(this), mEffect1(this), mEffect2(this) {}
    virtual ~dAcBomb_c() {}

    enum Type_e {
        BOMB_0 = 0, // Explode
        BOMB_1 = 1, // Flower
        BOMB_2 = 2, // Flower
        BOMB_3 = 3, // Carry
        BOMB_4 = 4, // Explode
        BOMB_5 = 5, // Flower
        BOMB_6 = 6, // Explode
        BOMB_7 = 7, // Carry
        BOMB_MAX,
    };

    enum Flag_e {
        FLAG_0x1 = (1 << 0), // Player Make
        FLAG_0x2 = (1 << 1),
        FLAG_0x4 = (1 << 2), // Bomb Hit ?
        FLAG_0x8 = (1 << 3),
        FLAG_0x10 = (1 << 4),
        FLAG_0x20 = (1 << 5), // Underwater?
        FLAG_0x40 = (1 << 6),
        FLAG_0x80 = (1 << 7),
        FLAG_0x100 = (1 << 8),
        FLAG_0x200 = (1 << 9),
        FLAG_0x400 = (1 << 10),
        FLAG_0x800 = (1 << 11),
        FLAG_0x1000 = (1 << 12),
        FLAG_0x2000 = (1 << 13),
        FLAG_0x4000 = (1 << 14),
        FLAG_0x8000 = (1 << 15),
        FLAG_0x10000 = (1 << 16),
        FLAG_0x20000 = (1 << 17),
        FLAG_0x40000 = (1 << 18),
        FLAG_0x80000 = (1 << 19),
        FLAG_0x100000 = (1 << 20),
        FLAG_0x200000 = (1 << 21),
        FLAG_0x400000 = (1 << 22),
        FLAG_0x800000 = (1 << 23),
        FLAG_0x1000000 = (1 << 24),
        FLAG_0x2000000 = (1 << 25),
        FLAG_0x4000000 = (1 << 26),
        FLAG_0x8000000 = (1 << 27),
        FLAG_0x10000000 = (1 << 28),
        FLAG_0x20000000 = (1 << 29),
        FLAG_0x40000000 = (1 << 30),
        FLAG_0x80000000 = (1 << 31),
    };

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

    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcBomb_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcBomb_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcBomb_c);
    STATE_MGR_DEFINE_UTIL_GETOLDSTATEID(dAcBomb_c);
    STATE_MGR_DEFINE_UTIL_GETNEWSTATEID(dAcBomb_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcBomb_c);

    void coHitCallback(cCcD_Obj *i_objInfB);
    void tgHitCallback(dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfA, cCcD_Obj *i_objInfB);

    void setTarget(const mVec3_c &t);

    void setRoomId();
    void setSmokePos();
    bool checkWaterIn();
    void setTransformFromFlower(const mMtx_c &);
    void setPickupTarget();
    bool checkExplode();
    void lookAt();
    void bounceWall(mAng angle);
    void fn_8025A1F0(mVec3_c &out);
    void setBombColor();
    void rollTo(const mVec3_c &dir);
    void rollToInternal(const mVec3_c &dir);

    s16 get0xA44() const {
        return mFuseTime;
    }

    bool check0xA3C(u32 mask) const {
        return (field_0xA3C & mask);
    }
    void unset0xA3C(u32 mask) {
        field_0xA3C &= ~mask;
    }
    void set0xA3C(u32 mask) {
        field_0xA3C |= mask;
    }

    u32 getBombType() const {
        return mParams;
    }

private:
    bool createModel();

public:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShdw;
    /* 0x358 */ dBgS_BombAcch mAcch;
    /* 0x708 */ dBgS_AcchCir mAcchCir;
    /* 0x764 */ dCcD_Sph mSph;
    /* 0x8B4 */ mMtx_c mMtx;
    /* 0x8E4 */ dBgS_BombLinChk mLinChk;
    /* 0x97C */ cBgS_PolyInfo mUnkInfo;
    /* 0x9A8 */ dAcObjRef_unk mUnkRef;
    /* 0x9D4 */ dEmitter_c mEffect1;
    /* 0xA08 */ dEmitter_c mEffect2;
    /* 0xA3C */ u32 field_0xA3C;
    /* 0xA40 */ u8 field_0xA40;
    /* 0xA41 */ u8 mMainMatID;
    /* 0xA42 */ u8 field_0xA42;
    /* 0xA44 */ s16 mFuseTime;
    /* 0xA46 */ u16 field_0xA46;
    /* 0xA48 */ mAng field_0xA48;
    /* 0xA4A */ mAng mBombRotationY;
    /* 0xA4C */ f32 mBombSpeed;
    /* 0xA50 */ f32 field_0xA50;
    /* 0xA54 */ mVec3_c field_0xA54;
    /* 0xA60 */ mVec3_c field_0xA60;
    /* 0xA6C */ mVec3_c field_0xA6C;
    /* 0xA78 */ mVec3_c field_0xA78;
    /* 0xA84 */ mVec3_c field_0xA84;
    /* 0xA90 */ mVec3_c field_0xA90;
    /* 0xA9C */ STATE_MGR_DECLARE(dAcBomb_c);
};

#endif
