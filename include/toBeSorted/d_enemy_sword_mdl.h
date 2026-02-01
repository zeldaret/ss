#ifndef D_ENEMY_SWORD_MDL_H
#define D_ENEMY_SWORD_MDL_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_cc.h"
#include "d/d_shadow.h"
#include "egg/gfx/eggTexture.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"
#include "toBeSorted/d_sword_swing_effect_mgr.h"

class dEnemySwordMdl_c {
public:
    /* 0x000 */ m3d::smdl_c mMdl;
    /* 0x01C */ dSwordSwingEffectProcMgr_c mProc;

private:
    /* 0x070 */ bool mIsActive;
    /* 0x071 */ bool mFirstFramePassed;
    /* 0x074 */ mVec3_c field_0x074;
    /* 0x080 */ mVec3_c field_0x080;
    /* 0x08C */ mVec3_c field_0x08C;
    /* 0x098 */ mVec3_c field_0x098;
    /* 0x0A4 */ f32 field_0x0A4;
    /* 0x0A8 */ f32 field_0x0A8;
    /* 0x0AC */ dCcD_Linked_Cps mCcs[3];

public:
    /* 0x52C */ dColliderLinkedList mCcList;

public:
    dEnemySwordMdl_c() {}
    virtual ~dEnemySwordMdl_c() {}

    bool create(
        mAllocator_c *alloc, void *pData, const char *mdlName, u32 bufferOption, const mVec3_c &v1, const mVec3_c &v2,
        cCcD_Stts &stts, EGG::ResTIMG *pImg, int nView, u32 *pSize
    );
    bool create(
        mAllocator_c *alloc, nw4r::g3d::ResFile resFile, const char *mdlName, u32 bufferOption, const mVec3_c &v1,
        const mVec3_c &v2, cCcD_Stts &stts, EGG::ResTIMG *pImg, int nView, u32 *pSize
    );

    void calc(const mMtx_c &mtx, const mVec3_c &v1, bool mass);
    void calc(const mMtx_c &mtx, mAng angle, bool mass) {
        // A bit of a wild inline but this vector is very low on the dAcBlastboss_c::actorExecute stack
        mVec3_c v1 = mVec3_c(angle.sin(), 0.0f, angle.cos());
        calc(mtx, v1, mass);
    }
    bool entry(dAcObjBase_c *obj, dShadowCircle_c *shadow, mQuat_c *quat);

    void enableAttack();
    void fn_8006B660(UNKWORD, UNKWORD, UNKWORD, UNKWORD, UNKWORD, UNKWORD, UNKWORD, UNKWORD, UNKWORD, f32);
    void setDamageMaybe(u8 damage);
    void fn_8006B7A0(u32);
    void fn_8006B800(u32);

    void enable() {
        enableAttack();
        mProc.setActive(true);
    }

    void disable() {
        mIsActive = false;
        mProc.setActive(false);
    }
};

#endif
