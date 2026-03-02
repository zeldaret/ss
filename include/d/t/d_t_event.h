#ifndef D_T_EVENT_H
#define D_T_EVENT_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "toBeSorted/actor_event.h"

class dTgEvent_c : public dTg_c {
public:
    dTgEvent_c() : mActorEvent(*this, nullptr) {}
    virtual ~dTgEvent_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    bool checkUncommittedFlag(u16 flag);
    bool isValidFlag(u16 flag);

private:
    /* 0x0FC */ ActorEventRelated mActorEvent;
    /* 0x14C */ mMtx_c mMatrix;
    /* 0x17C */ u32 mEventId;
    /* 0x180 */ u32 mSetSceneflag;
    /* 0x184 */ u32 mTrigSceneflag;
    /* 0x188 */ u32 mSubtype;
    /* 0x18C */ u32 mUnkParamFlag;
};
#endif
