#ifndef D_A_E_AM_H
#define D_A_E_AM_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEAm_c : public dAcEnBase_c {
public:
    dAcEAm_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEAm_c() {}

    STATE_FUNC_DECLARE(dAcEAm_c, Stop);
    STATE_FUNC_DECLARE(dAcEAm_c, Appear);
    STATE_FUNC_DECLARE(dAcEAm_c, StartWait);
    STATE_FUNC_DECLARE(dAcEAm_c, Normal);
    STATE_FUNC_DECLARE(dAcEAm_c, RetMove);
    STATE_FUNC_DECLARE(dAcEAm_c, LongJump);
    STATE_FUNC_DECLARE(dAcEAm_c, Wait);
    STATE_FUNC_DECLARE(dAcEAm_c, Chance);
    STATE_FUNC_DECLARE(dAcEAm_c, ChanceWait);
    STATE_FUNC_DECLARE(dAcEAm_c, Damage);
    STATE_FUNC_DECLARE(dAcEAm_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEAm_c);
};

#endif
