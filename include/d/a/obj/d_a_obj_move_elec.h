#ifndef D_A_OBJ_MOVE_ELEC_H
#define D_A_OBJ_MOVE_ELEC_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmmdl.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcOMoveElec_c : public dAcObjBase_c {
public:
    dAcOMoveElec_c() : mEvent(*this, nullptr), mResFile(nullptr), mStateMgr(*this) {}
    virtual ~dAcOMoveElec_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOMoveElec_c, WaitOff);
    STATE_FUNC_DECLARE(dAcOMoveElec_c, WaitOn);
    STATE_FUNC_DECLARE(dAcOMoveElec_c, Move);

private:
    void initPhase();

    /* 0x330 */ ActorEventRelated mEvent;
    /* 0x380 */ void *mResFile;
    /* 0x384 */ m3d::mdlAnmChr mMdl;
    /* 0x3EC */ dBgW mBgW;
    /* 0x5FC */ dBgW mBgWGround;
    /* 0x80C */ dCcD_Unk mCc;
    /* 0xA14 */ dBgS_AcchCir mAcchCir;
    /* 0xA70 */ dBgS_ObjAcch mAcch;
    /* 0xE20 */ STATE_MGR_DECLARE(dAcOMoveElec_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOMoveElec_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOMoveElec_c);
    /* 0xE5C */ dEmitter_c mEffect;
    /* 0xE90 */ f32 mMoveDist;
    /* 0xE94 */ s32 mPhase;
    /* 0xE98 */ u16 mMoveTimer;
    /* 0xE9A */ u8 mCcTimer;
    /* 0xE9B */ u8 mEventTimer;
    /* 0xE9C */ u8 mEventId;
    /* 0xE9D */ u8 mEventSceneflag;
    /* 0xE9E */ u8 mSceneflag;
    /* 0xE9F */ bool mIsShort;
};

#endif
