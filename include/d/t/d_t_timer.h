#ifndef D_T_TIMER_H
#define D_T_TIMER_H

#include <d/a/d_a_base.h>
#include <d/t/d_tg.h>

class dTgTimer_c : public dTg_c {
public:
    dTgTimer_c() {}
    virtual ~dTgTimer_c() {}

    virtual int create() override;
    virtual int actorExecute() override;

    int getSubtypeFromParams();
    u16 getTimerFromParams();

    u16 getCheckSceneflag();
    u16 getSetSceneflag();
    u16 getTimer();
    void setTimer(u16 val);
    u16 getTargetTime();
    bool checkShouldTrigger();
    u16 getStoredTargetTime();

    u16 getConstant0x50();
    void incrementTimer();
    void resetTimer();
    u16 getConstant0x50_Thunk();

    u16 (dTgTimer_c::*mGetTargetTimeFunc)();
    u16 mTimer;
    u16 mTargetTime;
};

#endif
