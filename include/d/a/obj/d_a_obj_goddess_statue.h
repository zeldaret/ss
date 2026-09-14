#ifndef D_A_OBJ_GODDESS_STATUE_H
#define D_A_OBJ_GODDESS_STATUE_H

#include "d/a/obj/d_a_obj_sw_sword_beam.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dAcOGoddessStatue_c : public dAcGoddessCrestHolder_c {
public:
    dAcOGoddessStatue_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOGoddessStatue_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;
    virtual void vt_0x88() override;

    STATE_FUNC_DECLARE(dAcOGoddessStatue_c, Wait);

private:
    ActorEventRelated mEvent;
    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dBgW mBgW;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOGoddessStatue_c);
    mMtx_c mMtx;
    mVec3_c mScale2;
    u8 mSceneFlag;
    u8 mExitID;
    bool mCrestActivated;

    static f32 sSendToSwSB;
};

#endif
