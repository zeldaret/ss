#ifndef D_A_GIRAHIMU_SWORD_BEAM_H
#define D_A_GIRAHIMU_SWORD_BEAM_H

#include "c/c_lib.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/d_shadow.h"
#include "m/m_angle.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcGirahimuSwordBeam_c : public dAcObjBase_c {
public:
    dAcGirahimuSwordBeam_c() : mStateMgr(*this) {}
    virtual ~dAcGirahimuSwordBeam_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcGirahimuSwordBeam_c, BulletMove);
    STATE_FUNC_DECLARE(dAcGirahimuSwordBeam_c, Damage);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcGirahimuSwordBeam_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcGirahimuSwordBeam_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcGirahimuSwordBeam_c);

    void swordBeamReturn(s32);
    void startMoveEmitters();
    void startDisappearEmitter();
    void transformMatrix();
    void fn_240_11F0();
    void fn_240_1380();
    void fn_240_16C0();
    void fn_240_1710();
    void fn_240_17C0(s32);
    void fn_240_1810(u32, mAng3_c &, const mAng &);

    void setB00(s16 x, s16 y, s16 z) {
        field_0xB00 = x;
        field_0xB02 = y;
        field_0xB04 = z;
    }

    mAng getLinkedAngle() const {
        return (cLib::targetAngleY(mPosition, field_0xAF4.get()->mPosition));
    }

private:
    /* 0x330 */ dShadowCircle_c mShadow;
    /* 0x338 */ dBgS_AcchCir mAcchCir;
    /* 0x394 */ dBgS_ObjAcch mAcch;
    /* 0x744 */ dCcD_Linked<dCcD_Cps> mCps0;
    /* 0x8C4 */ dCcD_Linked<dCcD_Cps> mCps1;
    /* 0xA40 */ dColliderLinkedList mCollider;
    /* 0xA50 */ STATE_MGR_DECLARE(dAcGirahimuSwordBeam_c);
    /* 0xA8C */ dEmitter_c mEmitter0;
    /* 0xAC0 */ dEmitter_c mEmitter1;
    /* 0xAF4 */ dAcObjRef_c field_0xAF4;
    /* 0xB00 */ s16 field_0xB00;
    /* 0xB02 */ s16 field_0xB02;
    /* 0xB04 */ s16 field_0xB04;
    /* 0xB06 */ s16 field_0xB06;
    /* 0xB08 */ s16 field_0xB08;
    /* 0xB0A */ s16 field_0xB0A;
    /* 0xB0C */ f32 field_0xB0C;
    /* 0xB10 */ s32 field_0xB10;
    /* 0xB14 */ mAng field_0xB14;
    /* 0xB16 */ s16 field_0xB16;
    /* 0xB18 */ u16 field_0xB18;
    /* 0xB1C */ s32 field_0xB1C;
    /* 0xB20 */ s32 field_0xB20;
    /* 0xB24 */ u8 field_0xB24;
    /* 0xB25 */ u8 field_0xB25;
    /* 0xB26 */ u8 field_0xB26;
    /* 0xB27 */ u8 field_0xB27;
};

#endif
