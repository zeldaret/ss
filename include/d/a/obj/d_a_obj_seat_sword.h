#ifndef D_A_OBJ_SEAT_SWORD_H
#define D_A_OBJ_SEAT_SWORD_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/flag/storyflag_manager.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtx.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

class dAcOSeatSword_c : public dAcObjBase_c {
public:
    dAcOSeatSword_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOSeatSword_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void registerInEvent() override;
    virtual void doInteraction(s32) override;

    STATE_FUNC_DECLARE(dAcOSeatSword_c, Wait);
    STATE_FUNC_DECLARE(dAcOSeatSword_c, Get);

    void actorExecuteCommon();
    void updateSwordMdl();
    bool checkPlayerHasSword();

    u8 getSubtype() const {
        return mParams & 0x3;
    }

private:
    /* 0x330 */ ActorEventRelated mEvent;
    /* 0x380 */ nw4r::g3d::ResFile mRes;
    /* 0x384 */ m3d::smdl_c mSwordMdl;
    /* 0x3A0 */ m3d::smdl_c mPedestalMdl;
    /* 0x3BC */ m3d::smdl_c mPedestalMdl_s;
    /* 0x3D8 */ dBgW mBgW;
    /* 0x5E8 */ dCcD_Cyl mCyl;
    /* 0x738 */ STATE_MGR_DECLARE(dAcOSeatSword_c);
    /* 0x774 */ dEmitter_c mEff;
    /* 0x7A8 */ mMtx_c mSwordMtx;
    /* 0x7D8 */ mVec3_c mEffPos;
    /* 0x7E4 */ f32 field_0x7E4;
    /* 0x7E8 */ mAng field_0x7E8;
    /* 0x7EA */ u8 field_0x7EA;
    /* 0x7EB */ u8 mSubtype;
    /* 0x7EC */ bool mbNoSword;

    static dCcD_SrcCyl sCylSrc;
    static const InteractionTargetDef sInteractionDef;
    static const Vec sEffectPos;
};

#endif
