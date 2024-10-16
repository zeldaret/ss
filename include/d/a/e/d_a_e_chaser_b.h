#ifndef D_A_E_CHASER_B_H
#define D_A_E_CHASER_B_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEChb_c : public dAcEnBase_c {
public:
    dAcEChb_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEChb_c() {}

    STATE_FUNC_DECLARE(dAcEChb_c, Standby);
    STATE_FUNC_DECLARE(dAcEChb_c, Chase);
    STATE_FUNC_DECLARE(dAcEChb_c, Start);
    STATE_FUNC_DECLARE(dAcEChb_c, Attack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEChb_c);
};

#endif
