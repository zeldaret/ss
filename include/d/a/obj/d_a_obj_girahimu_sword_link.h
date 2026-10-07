#ifndef D_A_OBJ_GIRAHIMU_SWORD_LINK_H
#define D_A_OBJ_GIRAHIMU_SWORD_LINK_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/d_shadow.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_enemy_sword_mdl.h"

class dAcObjGirahimuSwordLink_c : public dAcObjBase_c {
public:
    friend class dAcGirahimuBase_c;
    friend class dAcGirahimu_c;

    dAcObjGirahimuSwordLink_c() : mEmitter(this), mStateMgr(*this) {}
    virtual ~dAcObjGirahimuSwordLink_c() {}

    virtual int create() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    void init();

    void setEquip();
    void setGetSword();
    void setHide();
    void setThrow(const mVec3_c &force, mAng angle, f32 rate);
    void setAtThrow(const mVec3_c &velocity);
    void fn_238_19D0();
    void updateModelMatrix();
    void setTransform(const mMtx_c &mtx, const mVec3_c &pos);
    bool isHide();
    bool isStick();
    void reflect(const mVec3_c &rot, f32 gravity, f32, f32 force);
    bool lineCheck();

    dEnemySwordMdl_c &getMdl() {
        return mMdl;
    }

    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Hide);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Equip);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, GetSword);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Throw);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, AtThrow);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Stick);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Reflect);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcObjGirahimuSwordLink_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcObjGirahimuSwordLink_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcObjGirahimuSwordLink_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcObjGirahimuSwordLink_c);

private:
    /* 0x330 */ dEnemySwordMdl_c mMdl;
    /* 0x86C */ dShadowCircle_c mShadow;
    /* 0x874 */ dEmitter_c mEmitter;

    /* 0x8A8 */ s16 field_0x8A8;
    /* 0x8AA */ s16 field_0x8AA;
    /* 0x8AC */ u16 mTrailEmitterID;
    /* 0x8B0 */ mMtx_c field_0x8B0;
    /* 0x8E0 */ dBgS_AcchCir mAcchCir;
    /* 0x93C */ dBgS_ObjAcch mAcch;

    /* 0xCEC */ mVec3_c mGhirahimPos;
    /* 0xCF8 */ mAng3_c mAngleCopy;
    /* 0xD00 */ mQuat_c field_0xD00;
    /* 0xD10 */ mAng field_0xD10;
    /* 0xD14 */ mVec3_c field_0xD14;
    /* 0xD20 */ u8 field_0xD20[4];
    /* 0xD24 */ mAng field_0xD24;
    /* 0xD26 */ bool field_0xD26;
    /* 0xD28 */ STATE_MGR_DECLARE(dAcObjGirahimuSwordLink_c);
};

#endif
