#ifndef D_A_OBJ_SHED_H
#define D_A_OBJ_SHED_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/actor_event.h"

class dAcOShed_c : public dAcObjBase_c {
public:
    dAcOShed_c() : mEvent(*this, nullptr), mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOShed_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOShed_c, Wait);
    STATE_FUNC_DECLARE(dAcOShed_c, Move);

private:
    /* 0x330 */ ActorEventRelated mEvent;
    /* 0x380 */ nw4r::g3d::ResFile mResFile;
    /* 0x384 */ m3d::smdl_c mMdl;
    /* 0x3A0 */ dBgW mBgW;
    /* 0x5B0 */ STATE_MGR_DECLARE(dAcOShed_c);
    /* 0x5EC */ mVec3_c mVec;
    /* 0x5F8 */ bool mIsOpen;
    /* 0x5F9 */ u8 mSceneFlag;
    /* 0x5FA */ u8 mEventId;
    /* 0x5FB */ u8 mMoveTimer;
};

#endif
