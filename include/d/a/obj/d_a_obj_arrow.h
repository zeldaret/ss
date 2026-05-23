#ifndef D_A_OBJ_ARROW_H
#define D_A_OBJ_ARROW_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcArrow_c : public dAcObjBase_c {
public:
    dAcArrow_c() : mPolyInfo(), mStateMgr(*this), mEffects(this) {}
    virtual ~dAcArrow_c();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcArrow_c, Wait);
    STATE_FUNC_DECLARE(dAcArrow_c, Move);
    STATE_FUNC_DECLARE(dAcArrow_c, ActorStop);
    STATE_FUNC_DECLARE(dAcArrow_c, BgStop);
    STATE_FUNC_DECLARE(dAcArrow_c, Bound);

    void hitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB);

private:
    static cCcD_SrcGObj sCcSrcInf;
    static dCcD_SrcCps sCc1;
    static const dCcD_SrcSph sCc2;

    static const u8 sDamageArrMaybe[];
    static u16 sCounter;

    bool fn_8025DD20(const mVec3_c &v1, mVec3_c &v2, bool noEffect);
    void fn_8025DED0(f32 f);
    void fn_8025E160();
    void updateRoomId();
    s16 fn_8025E640();
    void fn_8025E720(dAcObjBase_c *obj, const mVec3_c &v);
    void updateMtx();
    void setInitialPosition();
    dAcObjBase_c *fn_8025E960();
    void initPickupCc();
    bool checkPickup();
    void fn_80260050(const mVec3_c &v, bool);
    void fn_802600D0(bool);
    void fn_802601C0();
    bool fn_80260250(dAcObjBase_c *o1, dAcObjBase_c *o2);

    enum ArrowFlag_e {
        ARROW_0x1 = 1 << 0,
        ARROW_0x2 = 1 << 1,
        ARROW_INITIAL_SPAWN = 1 << 2,
        ARROW_0x8 = 1 << 3,
        ARROW_0x10 = 1 << 4,
        ARROW_0x20 = 1 << 5,
        ARROW_0x40 = 1 << 6,
        ARROW_0x80 = 1 << 7,
        ARROW_0x100 = 1 << 8,
        ARROW_0x200 = 1 << 9,
        ARROW_0x400 = 1 << 10,
        ARROW_0x800 = 1 << 11,
        ARROW_0x1000 = 1 << 12,
        ARROW_0x2000 = 1 << 13,
        ARROW_0x4000 = 1 << 14,
        ARROW_0x8000 = 1 << 15,
        ARROW_0x10000 = 1 << 16,
    };

    void setArrowFlag(u32 f) {
        mArrowFlags |= f;
    }

    void unsetArrowFlag(u32 f) {
        mArrowFlags &= ~f;
    }

    bool checkArrowFlag (u32 f) const {
        return (mArrowFlags & f) != 0;
    }

    enum ArrowType_e {
        TYPE_MSK_SLINGSHOT = 0x10,
    };

    bool isSlingshotProjectile() const {
        return (mArrowType & TYPE_MSK_SLINGSHOT) != 0;
    }

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ dAcRef_c<dAcObjBase_c> mRef1;
    /* 0x35C */ dCcD_Cps mCcCps;
    /* 0x4CC */ dCcD_Sph mCcSph;
    /* 0x61C */ cBgS_PolyInfo mPolyInfo; // ??
    /* 0x648 */ dEmitter_c mEffects;
    /* 0x67C */ u32 mArrowFlags;
    /* 0x680 */ u8 mArrowType;
    /* 0x681 */ u8 field_0x681;
    /* 0x682 */ u8 mDespawnTimer;
    /* 0x683 */ u8 field_0x683;
    /* 0x684 */ s16 field_0x684;
    /* 0x686 */ u8 field_0x686[0x688 - 0x686];
    /* 0x688 */ s16 field_0x688;
    /* 0x68A */ s16 field_0x68A;
    /* 0x68C */ u16 mEffectId;
    /* 0x68E */ mAng field_0x68E;
    /* 0x690 */ mAng field_0x690;
    /* 0x692 */ mAng3_c field_0x692;
    /* 0x698 */ UNKWORD field_0x698;
    /* 0x69C */ f32 field_0x69C;
    /* 0x6A0 */ f32 field_0x6A0;
    /* 0x6A4 */ f32 field_0x6A4;
    /* 0x6A8 */ f32 field_0x6A8;
    /* 0x6AC */ f32 field_0x6AC;
    /* 0x6B0 */ mVec3_c field_0x6B0;
    /* 0x6BC */ mVec3_c field_0x6BC;
    /* 0x6C8 */ mVec3_c field_0x6C8;
    /* 0x6D4 */ mVec3_c field_0x6D4;
    /* 0x6E0 */ mVec3_c field_0x6E0;
    /* 0x6EC */ mVec3_c field_0x6EC;
    /* 0x6F8 */ dAcRef_c<dAcObjBase_c> mRef2;
    /* 0x704 */ STATE_MGR_DECLARE(dAcArrow_c);
};

#endif
