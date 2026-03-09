#ifndef D_T_EVENT_F_H
#define D_T_EVENT_F_H

#include "common.h"
#include "d/flag/storyflag_manager.h"
#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "toBeSorted/actor_event.h"

class dTgEventF_c : public dTg_c {
public:
    dTgEventF_c() : mActorEvent(*this, nullptr) {}
    virtual ~dTgEventF_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    static int checkStoryflagValid(u32);
    static bool checkUncommitedStoryFlagValue(u32);
    static void setStoryFlag(StoryFlags_e);
    static void unsetStoryFlag(StoryFlags_e);
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

private:
    /* 0x0FC */ ActorEventRelated mActorEvent;
    /* 0x14C */ mMtx_c mMatrix;
    /* 0x17C */ u32 mEventId;
    /* 0x180 */ u32 mSetStoryflag;
    /* 0x184 */ u32 mTrigStoryflag;
    /* 0x188 */ u32 mSubtype;
};
#endif
