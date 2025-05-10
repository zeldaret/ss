#ifndef D_A_OBJ_APPEAR_BRIDGE_H
#define D_A_OBJ_APPEAR_BRIDGE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcOappearBridge_c : public dAcObjBase_c {
public:
    dAcOappearBridge_c() : mStateMgr(*this, sStateID::null), mActorEvent(*this, nullptr), mSceneCallback(this) {}
    virtual ~dAcOappearBridge_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dAcOappearBridge_c, Wait);
    STATE_FUNC_DECLARE(dAcOappearBridge_c, Appear);
    STATE_FUNC_DECLARE(dAcOappearBridge_c, Disappear);

private:
    static const f32 sMovementRate;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mModel;
    m3d::anmTexSrt_c mSrtAnm;
    m3d::anmMatClr_c mClrAnm;
    dBgW mCollision;
    STATE_MGR_DECLARE(dAcOappearBridge_c);
    ActorEventRelated mActorEvent;

    // TODO the weak IScnObjCallback functions are in the wrong order.
    // The vtable order is            dtor, World,   Mat,  View,   Opa,   Xlu
    // and the emitted order is       dtor,   Xlu,   Opa,  View,   Mat, World
    // but the order in the binary is dtor,   Xlu,   Opa,  Mat,  World,  View
    dScnCallback_c mSceneCallback;
    /* 0x650 */ mVec3_c mSoundPosition;
    /* 0x65C */ f32 field_0x65C;
    /* 0x660 */ u8 mAreaIdx;
    /* 0x661 */ u8 mEventId;
};

#endif
