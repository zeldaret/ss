#ifndef D_A_E_ST_WIRE_H
#define D_A_E_ST_WIRE_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEstwire_c : public dAcEnBase_c {
public:
    dAcEstwire_c() : mStateMgr(*this) {}
    virtual ~dAcEstwire_c() {}

    STATE_FUNC_DECLARE(dAcEstwire_c, Shoot);
    STATE_FUNC_DECLARE(dAcEstwire_c, Fix);
    STATE_FUNC_DECLARE(dAcEstwire_c, Out);
    STATE_FUNC_DECLARE(dAcEstwire_c, AirBack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEstwire_c);
};

#endif
