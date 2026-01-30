#ifndef D_T_ROLL_ATTACK_LOG_H
#define D_T_ROLL_ATTACK_LOG_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgRollAttackLog_c : public dTg_c {
public:
    dTgRollAttackLog_c() : mStateMgr(*this) {}
    virtual ~dTgRollAttackLog_c() {}

    STATE_FUNC_DECLARE(dTgRollAttackLog_c, Wait);
    STATE_FUNC_DECLARE(dTgRollAttackLog_c, RollAttack);
    STATE_FUNC_DECLARE(dTgRollAttackLog_c, Request);
    STATE_FUNC_DECLARE(dTgRollAttackLog_c, FinishAttack);
    STATE_FUNC_DECLARE(dTgRollAttackLog_c, None);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgRollAttackLog_c);
};

#endif
