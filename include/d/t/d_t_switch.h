#ifndef D_T_SWITCH_H
#define D_T_SWITCH_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dTgSw_c : public dTg_c {
public:
    dTgSw_c() : mStateMgr(*this), mActorEvent(*this, nullptr) {}
    virtual ~dTgSw_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dTgSw_c, OnWait);
    STATE_FUNC_DECLARE(dTgSw_c, On);
    STATE_FUNC_DECLARE(dTgSw_c, OffWait);
    STATE_FUNC_DECLARE(dTgSw_c, Off);

private:
    bool isLmfBossKeyPuzzle();
    void resetAllSwitches();
    void resetSwitchesAfterCurrent();
    bool isPuzzleSolved();

    /* 0x0FC */ STATE_MGR_DECLARE(dTgSw_c);
    /* 0x138 */ ActorEventRelated mActorEvent;
    /* 0x188 */ u8 mSetSceneFlagId;
    /* 0x189 */ u8 mTrigSceneFlagIdBegin;
    /* 0x18A */ u8 mFirst2;
    /* 0x18B */ u8 mSecond2;
    /* 0x18C */ u8 mThird2;
    /* 0x18D */ u8 mNumSwitchesToWin;
    /* 0x18E */ u8 mTrigSceneFlagIdEnd;
    /* 0x18F */ u8 mTimer;
    /* 0x190 */ u8 mCurrentFlagPosition;
    /* 0x191 */ bool mIsFsBossKeyPuzzle;
    /* 0x192 */ u8 field_0x192;
};

#endif
