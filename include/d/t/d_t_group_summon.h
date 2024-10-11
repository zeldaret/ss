#ifndef D_T_GROUP_SUMMON_H
#define D_T_GROUP_SUMMON_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgGroupSummon_c : public dAcBase_c {
public:
    dTgGroupSummon_c() : mStateMgr(*this, sStateID::null) {}
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
