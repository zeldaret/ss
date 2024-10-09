#ifndef D_A_B_MG_H
#define D_A_B_MG_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBmg_c : public dAcEnBase_c {
public:
    dAcBmg_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBmg_c() {}

    STATE_FUNC_DECLARE(dAcBmg_c, Start);
    STATE_FUNC_DECLARE(dAcBmg_c, DiveWait);
    STATE_FUNC_DECLARE(dAcBmg_c, Dive);
    STATE_FUNC_DECLARE(dAcBmg_c, DiveWalk);
    STATE_FUNC_DECLARE(dAcBmg_c, Fight);
    STATE_FUNC_DECLARE(dAcBmg_c, HandAttack);
    STATE_FUNC_DECLARE(dAcBmg_c, TailAttack);
    STATE_FUNC_DECLARE(dAcBmg_c, DiveAttack);
    STATE_FUNC_DECLARE(dAcBmg_c, LostArm);
    STATE_FUNC_DECLARE(dAcBmg_c, MiddleDemo);
    STATE_FUNC_DECLARE(dAcBmg_c, ArmDamage);
    STATE_FUNC_DECLARE(dAcBmg_c, CoreDamage);
    STATE_FUNC_DECLARE(dAcBmg_c, EyeBind);
    STATE_FUNC_DECLARE(dAcBmg_c, BombDamage);
    STATE_FUNC_DECLARE(dAcBmg_c, VacuumDamage);
    STATE_FUNC_DECLARE(dAcBmg_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBmg_c);
};

#endif
