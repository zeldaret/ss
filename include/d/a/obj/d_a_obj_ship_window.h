#ifndef D_A_OBJ_SHIP_WINDOW_H
#define D_A_OBJ_SHIP_WINDOW_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dAcOshipWindow_c : public dAcObjBase_c {
public:
    dAcOshipWindow_c() : mStateMgr(*this), mEvent(*this, nullptr) {}
    virtual ~dAcOshipWindow_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOshipWindow_c, Close);
    STATE_FUNC_DECLARE(dAcOshipWindow_c, Move);
    STATE_FUNC_DECLARE(dAcOshipWindow_c, Open);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdls[2];
    /* 0x36C */ dBgW mBgW;
    /* 0x57C */ STATE_MGR_DECLARE(dAcOshipWindow_c);
    /* 0x5B8 */ ActorEventRelated mEvent;
    /* 0x608 */ f32 mPastState;
    /* 0x60C */ u8 mOpenSceneflag;
    /* 0x60D */ u8 mEventDoneSceneflag; // Set after the open event, +1 after the close event
    /* 0x60E */ u8 mOpenEventId;
    /* 0x60F */ u8 mCloseEventId;
    /* 0x610 */ s16 mMoveTimer;
    /* 0x612 */ bool mOpenEventDone;
    /* 0x613 */ bool mCloseEventDone;
    /* 0x614 */ bool mEventOnce;
};

#endif
