#ifndef D_A_E_HIDOKARI_H
#define D_A_E_HIDOKARI_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEhidokari_c : public dAcEnBase_c {
public:
    dAcEhidokari_c() : mStateMgr(*this) {}
    virtual ~dAcEhidokari_c() {}

    STATE_FUNC_DECLARE(dAcEhidokari_c, Wait);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Walk);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Kyoro);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Find);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Run);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Inside);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Stun);
    STATE_FUNC_DECLARE(dAcEhidokari_c, WaitShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, RunShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, WalkShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, FireShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, RetShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, NoShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Damage);
    STATE_FUNC_DECLARE(dAcEhidokari_c, GuradShell);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Panic);
    STATE_FUNC_DECLARE(dAcEhidokari_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEhidokari_c);
};

#endif
