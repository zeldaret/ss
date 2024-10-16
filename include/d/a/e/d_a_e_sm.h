#ifndef D_A_E_SM_H
#define D_A_E_SM_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEsm_c : public dAcEnBase_c {
public:
    dAcEsm_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEsm_c() {}

    STATE_FUNC_DECLARE(dAcEsm_c, BaseMother);
    STATE_FUNC_DECLARE(dAcEsm_c, Wait);
    STATE_FUNC_DECLARE(dAcEsm_c, Walk);
    STATE_FUNC_DECLARE(dAcEsm_c, BirthJump);
    STATE_FUNC_DECLARE(dAcEsm_c, Shake);
    STATE_FUNC_DECLARE(dAcEsm_c, Attack);
    STATE_FUNC_DECLARE(dAcEsm_c, Electrical);
    STATE_FUNC_DECLARE(dAcEsm_c, Absorption);
    STATE_FUNC_DECLARE(dAcEsm_c, Fusion);
    STATE_FUNC_DECLARE(dAcEsm_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEsm_c);
};

#endif
