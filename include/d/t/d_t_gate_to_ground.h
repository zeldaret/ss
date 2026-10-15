#ifndef D_T_GATE_TO_GROUND_H
#define D_T_GATE_TO_GROUND_H

#include "common.h"
#include "d/d_area.h"
#include "d/t/d_tg.h"
#include "toBeSorted/actor_event.h"

class dTgGateToGround_c : public dTg_c {
public:
    dTgGateToGround_c() : mEventRelated(*this, nullptr) {}
    virtual ~dTgGateToGround_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

private:
    /* 0xfc  */ ActorEventRelated mEventRelated;
    /* 0x14c */ dArea_c mArea;
    /* 0x17c */ s32 mDelayFrameCount;
    /* 0x180 */ s16 mStoryflag;
    /* 0x182 */ s8 field_0x182;
    /* 0x183 */ s8 mCameraID;
};

#endif
