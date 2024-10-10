#ifndef D_A_B_KR_H
#define D_A_B_KR_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBkr_c : public dAcEnBase_c {
public:
    dAcBkr_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBkr_c() {}

    STATE_FUNC_DECLARE(dAcBkr_c, StartDemo);
    STATE_FUNC_DECLARE(dAcBkr_c, UnderWater);
    STATE_FUNC_DECLARE(dAcBkr_c, Appear);
    STATE_FUNC_DECLARE(dAcBkr_c, Fight);
    STATE_FUNC_DECLARE(dAcBkr_c, Attack);
    STATE_FUNC_DECLARE(dAcBkr_c, AttackCrash);
    STATE_FUNC_DECLARE(dAcBkr_c, LastFight);
    STATE_FUNC_DECLARE(dAcBkr_c, LastDamage);
    STATE_FUNC_DECLARE(dAcBkr_c, ArmDamage);
    STATE_FUNC_DECLARE(dAcBkr_c, ArmDamage2);
    STATE_FUNC_DECLARE(dAcBkr_c, CoreDamage);
    STATE_FUNC_DECLARE(dAcBkr_c, SmallDamage);
    STATE_FUNC_DECLARE(dAcBkr_c, ArmRecover);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBkr_c);
};

#endif
