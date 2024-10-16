#ifndef D_A_E_GUE_H
#define D_A_E_GUE_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEgue_c : public dAcEnBase_c {
public:
    dAcEgue_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEgue_c() {}

    STATE_FUNC_DECLARE(dAcEgue_c, Attack);
    STATE_FUNC_DECLARE(dAcEgue_c, Blow);
    STATE_FUNC_DECLARE(dAcEgue_c, Damage);
    STATE_FUNC_DECLARE(dAcEgue_c, FlyHome);
    STATE_FUNC_DECLARE(dAcEgue_c, FlyPlayer);
    STATE_FUNC_DECLARE(dAcEgue_c, Return);
    STATE_FUNC_DECLARE(dAcEgue_c, Stay);
    STATE_FUNC_DECLARE(dAcEgue_c, Stun);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEgue_c);
};

#endif
