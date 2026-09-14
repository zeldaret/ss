#ifndef D_A_OBJ_SWORD_CANDLE_H
#define D_A_OBJ_SWORD_CANDLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_sw_sword_beam.h"
#include "d/col/bg/d_bg_w.h"
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

    STATE_FUNC_DECLARE(dAcOSwordCandle_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwordCandle_c, WaitClear);

private:
    ActorEventRelated mEvent;
    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dShadowCircle_c mShadow;
    dCcD_Cyl mCyl;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwordCandle_c);
    dBgW mBgW;
    dEmitter_c mEmitter;

    SHADOW_INFLUENCE field_0x7DC;

    mVec3_c field_0x7F0;

    u16 field_0x810;
    u8 mActivatedSceneflag;
    u8 mRiseFromGroundSceneflag;
    u8 field_0x814;

    u8 field_0x816;
    u8 field_0x818;

    static u16 lbl_530_bss_8[3];
    static dCcD_SrcCyl sCylSrc;
};

#endif
