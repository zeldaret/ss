#ifndef D_A_OBJ_STREAM_LAVA_H
#define D_A_OBJ_STREAM_LAVA_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcOstreamLava_c : public dAcObjBase_c {
public:
    dAcOstreamLava_c() : mStateMgr(*this, sStateID::null), mEvent(*this, nullptr), mScnCallback(nullptr) {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOstreamLava_c() {}
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOstreamLava_c, Wait);
    STATE_FUNC_DECLARE(dAcOstreamLava_c, Stream);

private:
    const static f32 unkFloat0;
    static u32 eventFlags1;
    static u32 eventFlags2;

    static void eventIn_Wrapper(void *arg);
    void eventIn();
    static void eventEnd_Wrapper(void *arg);
    void eventEnd();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ m3d::anmTexSrt_c mAnmTexSrtWait;
    /* 0x37C */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x3A8 */ dBgW mCollision;
    /* 0x5B8 */ STATE_MGR_DECLARE(dAcOstreamLava_c);
    /* 0x5F4 */ ActorEventRelated mEvent;
    /* 0x644 */ dScnCallback_c mScnCallback;

    /* 0x650 */ u8 mSubtype;
    /* 0x651 */ u8 mShouldStreamSceneflag;
    /* 0x652 */ u8 mEventId;
    /* 0x653 */ bool mHideActor;
};

#endif
