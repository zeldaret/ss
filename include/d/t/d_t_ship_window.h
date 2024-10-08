#ifndef D_T_SHIP_WINDOW_H
#define D_T_SHIP_WINDOW_H

#include <d/t/d_tg.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/actor_event.h>

class dTgShipWindow_c : public dTg_c {
public:
    dTgShipWindow_c() : mStateMgr(*this, sStateID::null), mEvent(*this, nullptr) {}
    virtual ~dTgShipWindow_c() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgShipWindow_c, On);
    STATE_FUNC_DECLARE(dTgShipWindow_c, Off);

private:
    static u32 sDefaultRotX;
    STATE_MGR_DECLARE(dTgShipWindow_c);
    ActorEventRelated mEvent;
    u8 mInitialStateFlag;
    u8 mOutputStateFlagBase;
    u8 mOffEventId;
    u8 mOnEventId;
    u8 mHasPlayedOffEvent;
    u8 mHasPlayedOnEvent;
    u8 mAlwaysPlayEvent;
};

#endif
