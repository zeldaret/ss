#ifndef D_T_GATE_TO_GROUND_H
#define D_T_GATE_TO_GROUND_H

#include "common.h"
#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "toBeSorted/actor_event.h"

class dTgGateToGround_c : public dTg_c {
public:
    dTgGateToGround_c() : mEventRelated(*this, nullptr) {}
    virtual ~dTgGateToGround_c() {}

    virtual int create() override;
    virtual int doDelete() override; // fn_205_150
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override; // fn_205_2F0

private:
    /* 0xfc  */ ActorEventRelated mEventRelated;
    /* 0x14c */ mMtx_c matrix;
    /* 0x17c */ int delayFrames;
    /* 0x180 */ s16 params_FF_FF_00_00;
    /* 0x182 */ s8 params_00_00_FF_00;
    /* 0x183 */ s8 params_00_00_00_FF;
};

#endif
