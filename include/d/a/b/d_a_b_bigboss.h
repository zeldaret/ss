#ifndef D_A_B_BIGBOSS_H
#define D_A_B_BIGBOSS_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcBBigBoss_c : public dAcEnBase_c {
public:
    dAcBBigBoss_c() : mStateMgr(*this) {}
    virtual ~dAcBBigBoss_c() {}

    STATE_FUNC_DECLARE(dAcBBigBoss_c, AppearWait);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, AppearDemo);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Wait);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Walk);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Reach);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Turn);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, ImpaAdvice);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, ShakeHead);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, HeadDamage);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, HeadDamageThrow);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, HeadPain);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StunPain);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, HeadRecover);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Attack);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, FrontFall);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Rush);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Rise);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Tied);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, LevelUp);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, FootBroken);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, FootRecover);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Stun);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StunWait);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StunRecover);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StunDamage);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StunFrontFall);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, Evacuation);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, StoneWait);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, SwordGameWait);
    STATE_FUNC_DECLARE(dAcBBigBoss_c, SwordGameEnd);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBBigBoss_c);
};

#endif
