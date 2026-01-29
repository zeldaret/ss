#ifndef D_A_E_OR_CANNON_H
#define D_A_E_OR_CANNON_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEorCannon_c : public dAcEnBase_c {
public:
    dAcEorCannon_c() : mStateMgr(*this) {}
    virtual ~dAcEorCannon_c() {}

    STATE_FUNC_DECLARE(dAcEorCannon_c, Wait);
    STATE_FUNC_DECLARE(dAcEorCannon_c, Attack);
    STATE_FUNC_DECLARE(dAcEorCannon_c, Death);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEorCannon_c);
};

#endif
