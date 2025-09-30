#ifndef D_A_OBJ_WARP_HOLE_H
#define D_A_OBJ_WARP_HOLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcOwarpHole_c : public dAcObjBase_c {
public:
    dAcOwarpHole_c() : mEvent(*this, nullptr) {}
    virtual ~dAcOwarpHole_c() {}

    virtual bool createHeap() override;
    virtual int create() override;

    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

private:
    /* 0x330 */ dCcD_Cyl mCcCyl;
    /* 0x480 */ EffectsStruct mEff;
    /* 0x4B4 */ ActorEventRelated mEvent;
    /* 0x504 */ mVec3_c mLinkPos;
    /* 0x510 */ u8 mExitListIdx;
    /* 0x511 */ u8 mWalkFramesMaybe;

    static dCcD_SrcCyl sCylSrc;
    static const float dummy600;
};

#endif
