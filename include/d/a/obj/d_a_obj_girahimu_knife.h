#ifndef D_A_OBJ_GIRAHIMU_KNIFE_H
#define D_A_OBJ_GIRAHIMU_KNIFE_H

#include "d/a/e/d_a_en_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcObjGirahimuKnife_c : public dAcEnBase_c {
public:
    dAcObjGirahimuKnife_c() : mGlowEmitter(this), mTrailEmitter(this), mStateMgr(*this) {}
    virtual ~dAcObjGirahimuKnife_c() {}

    virtual int create() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Wait);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinWait);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinWaitPreAttack);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinFreeWait);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, FreeWait);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Attack);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, AttackEnd);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Return);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Hit);
    STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, CircleWait);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcObjGirahimuKnife_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcObjGirahimuKnife_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcObjGirahimuKnife_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcObjGirahimuKnife_c);

    void init();

    void fn_239_1C40();
    void fn_239_1DB0(const mVec3_c &base, const mVec3_c &target);
    void fn_239_1F50(const mVec3_c &target, f32 add);
    void fn_239_2110(const mVec3_c &target, f32 add, s16 ang);
    void setTarget(const mVec3_c &t);
    void fn_239_2360(bool, bool, f32 zVel);
    void fn_239_2460(bool, bool, f32 velMult);
    bool isWaiting();
    bool isCircleWait();
    bool lineCheck();
    void updateMdlMatrix();
    void startEmitters();
    void fn_239_2B10();
    bool checkCutDir(s32 cutDir);
    void setWait();
    void setSpinWaitPreAttack();
    void getReflectForce(cCcD_Obj &cc, f32 forceMult, mVec3_c &outforce);
    void adjustVelocity(const mVec3_c &force, f32 yVel, f32 forwardVel);

private:
    /* 0x378 */ m3d::smdl_c mMdl;
    /* 0x394 */ dShadowCircle_c mShadow;
    /* 0x39C */ dEmitter_c mGlowEmitter;
    /* 0x3D0 */ dEmitter_c mTrailEmitter;
    /* 0x404 */ dCcD_Linked<dCcD_Sph> mSph0;
    /* 0x564 */ dCcD_Linked<dCcD_Sph> mSph1;
    /* 0x6C0 */ dColliderLinkedList mCollider;
    /* 0x6D0 */ s16 field_0x6D0;
    /* 0x6D2 */ u16 mGlowEmitterId;
    /* 0x6D4 */ u16 mTrailEmitterId;
    /* 0x6D6 */ u16 field_0x6D6;
    /* 0x6D8 */ mVec3_c field_0x6D8;
    /* 0x6E4 */ mVec3_c field_0x6E4;
    /* 0x6F0 */ mAng3_c field_0x6F0;
    /* 0x6F6 */ mAng3_c field_0x6F6;
    /* 0x7FC */ u8 _0x6FC[0x704 - 0x6FC];
    /* 0x704 */ mQuat_c field_0x704;
    /* 0x714 */ mMtx_c field_0x714;
    /* 0x744 */ mVec3_c field_0x744;
    /* 0x750 */ mVec3_c field_0x750;
    /* 0x75C */ mVec3_c field_0x75C;
    /* 0x768 */ mVec3_c field_0x768;
    /* 0x774 */ f32 field_0x774;
    /* 0x778 */ s16 field_0x778;
    /* 0x77C */ f32 field_0x77C;
    /* 0x780 */ mAng field_0x780;
    /* 0x784 */ s32 field_0x784;
    /* 0x788 */ s32 mState;
    /* 0x78C */ bool field_0x78C;
    /* 0x78D */ bool field_0x78D;
    /* 0x78E */ bool field_0x78E;
    /* 0x78F */ bool field_0x78F;
    /* 0x790 */ bool field_0x790;
    /* 0x791 */ bool field_0x791;
    /* 0x792 */ bool field_0x792;
    /* 0x793 */ bool field_0x793;
    /* 0x794 */ STATE_MGR_DECLARE(dAcObjGirahimuKnife_c);

    static s32 sSomething0;
    static s32 sSomething0_1;
    static mAng sSomething1;
    static s16 sSomething2;
};

#endif
