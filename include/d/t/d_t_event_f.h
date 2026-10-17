#ifndef D_T_EVENT_F_H
#define D_T_EVENT_F_H

#include "common.h"
#include "d/d_area.h"
#include "d/t/d_tg.h"
#include "toBeSorted/actor_event.h"

class dTgEventF_c : public dTg_c {
public:
    dTgEventF_c() : mActorEvent(*this, nullptr) {}
    virtual ~dTgEventF_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    static int checkStoryflagValid(s32);
    static bool checkUncommitedStoryFlagValue(s32);
    static void setStoryFlag(s32);
    static void unsetStoryFlag(s32);
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

private:
    /* 0x0FC */ ActorEventRelated mActorEvent;
    /* 0x14C */ dArea_c mArea;
    /* 0x17C */ u32 mEventId;
    /* 0x180 */ s32 mSetStoryflag;
    /* 0x184 */ s32 mTrigStoryflag;
    /* 0x188 */ u32 mSubtype;
};
#endif
