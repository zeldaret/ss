#ifndef D_A_E_HIDORY_H
#define D_A_E_HIDORY_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEHidory_c : public dAcEnBase_c {
public:
    dAcEHidory_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEHidory_c() {}

    STATE_FUNC_DECLARE(dAcEHidory_c, Wait);
    STATE_FUNC_DECLARE(dAcEHidory_c, Hovering);
    STATE_FUNC_DECLARE(dAcEHidory_c, Attack);
    STATE_FUNC_DECLARE(dAcEHidory_c, Whip_Damage);
    STATE_FUNC_DECLARE(dAcEHidory_c, Damage);
    STATE_FUNC_DECLARE(dAcEHidory_c, Death);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEHidory_c);
};

#endif
