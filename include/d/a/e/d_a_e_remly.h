#ifndef D_A_E_REMLY_H
#define D_A_E_REMLY_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEremly_c : public dAcEnBase_c {
public:
    dAcEremly_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEremly_c() {}

    STATE_FUNC_DECLARE(dAcEremly_c, Wait);
    STATE_FUNC_DECLARE(dAcEremly_c, Walk);
    STATE_FUNC_DECLARE(dAcEremly_c, Run);
    STATE_FUNC_DECLARE(dAcEremly_c, Escape);
    STATE_FUNC_DECLARE(dAcEremly_c, EscapeDash);
    STATE_FUNC_DECLARE(dAcEremly_c, Wind);
    STATE_FUNC_DECLARE(dAcEremly_c, Hold);
    STATE_FUNC_DECLARE(dAcEremly_c, Jump);
    STATE_FUNC_DECLARE(dAcEremly_c, Fly);
    STATE_FUNC_DECLARE(dAcEremly_c, Damage);
    STATE_FUNC_DECLARE(dAcEremly_c, Sleep);
    STATE_FUNC_DECLARE(dAcEremly_c, Scared);
    STATE_FUNC_DECLARE(dAcEremly_c, Stun);
    STATE_FUNC_DECLARE(dAcEremly_c, Water);
    STATE_FUNC_DECLARE(dAcEremly_c, Hear);
    STATE_FUNC_DECLARE(dAcEremly_c, NightSleepDemo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightFoo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightReflectionFoo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightWait);
    STATE_FUNC_DECLARE(dAcEremly_c, NightWalk);
    STATE_FUNC_DECLARE(dAcEremly_c, NightRun);
    STATE_FUNC_DECLARE(dAcEremly_c, NightRet);
    STATE_FUNC_DECLARE(dAcEremly_c, NightJumpAttack);
    STATE_FUNC_DECLARE(dAcEremly_c, BirthWait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEremly_c);
};

#endif
