#ifndef D_T_GROUP_SUMMON_H
#define D_T_GROUP_SUMMON_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgGroupSummon_c : public dTg_c {
public:
    dTgGroupSummon_c() : mStateMgr(*this) {}
    virtual ~dTgGroupSummon_c() {}

    STATE_FUNC_DECLARE(dTgGroupSummon_c, Wait);
    STATE_FUNC_DECLARE(dTgGroupSummon_c, PreSummon);
    STATE_FUNC_DECLARE(dTgGroupSummon_c, Summoning);
    STATE_FUNC_DECLARE(dTgGroupSummon_c, Watch);
    STATE_FUNC_DECLARE(dTgGroupSummon_c, Demo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgGroupSummon_c);
};

#endif
