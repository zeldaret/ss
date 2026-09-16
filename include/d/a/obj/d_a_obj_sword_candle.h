#ifndef D_A_OBJ_SWORD_CANDLE_H
#define D_A_OBJ_SWORD_CANDLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_sw_sword_beam.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcOSwordCandle_c : public dAcGoddessCrestHolder_c {
public:
    dAcOSwordCandle_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOSwordCandle_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorPostCreate() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;
    virtual int draw() override;
    virtual void vt_0x88(f32 &param) override;

    STATE_FUNC_DECLARE(dAcOSwordCandle_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwordCandle_c, WaitClear);

private:
    void fn_530_1180();

    /* 0x368 */ ActorEventRelated mEvent;
    /* 0x3B8 */ nw4r::g3d::ResFile mResFile;
    /* 0x3BC */ m3d::smdl_c mMdl;
    /* 0x3D8 */ dShadowCircle_c mShadow;
    /* 0x3E0 */ dCcD_Cyl mCyl;
    /* 0x530 */ STATE_MGR_DECLARE(dAcOSwordCandle_c);
    /* 0x56C */ dBgW mBgW;
    /* 0x77C */ dEmitter_c mEmitter;
    /* 0x7B0 */ cBgS_PolyInfo field_0x7B0;
    /* 0x7DC */ SHADOW_INFLUENCE field_0x7DC;
    /* 0x7F0 */ mVec3_c field_0x7F0;
    /* 0x7FC */ mVec3_c field_0x7FC;
    /* 0x808 */ float field_0x808;
    /* 0x80C */ float field_0x80C;
    /* 0x810 */ u16 field_0x810;
    /* 0x812 */ u8 mActivatedSceneflag;
    /* 0x813 */ u8 mRiseFromGroundSceneflag;
    /* 0x814 */ u8 field_0x814;
    /* 0x815 */ u8 mTimer;
    /* 0x816 */ u8 field_0x816;
    /* 0x817 */ u8 field_0x817;
    /* 0x818 */ u8 field_0x818;

    static u16 lbl_530_bss_8[6];
    static dCcD_SrcCyl sCylSrc;
};

#endif
