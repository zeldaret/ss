#ifndef D_A_AVATER_RACE_MNG_H
#define D_A_AVATER_RACE_MNG_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAvater_Race_Mng_c : public dAcBase_c {
public:
    dAvater_Race_Mng_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAvater_Race_Mng_c() {}

    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, StartWait);
    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, Race);
    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, Hide);
    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, Finish);
    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, LevelUpDemo);
    STATE_FUNC_DECLARE(dAvater_Race_Mng_c, Message);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAvater_Race_Mng_c);
};

#endif
