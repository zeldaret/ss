#ifndef D_T_SPARKS_H
#define D_T_SPARKS_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgSparksTag : public dAcBase_c {
public:
    dTgSparksTag() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgSparksTag() {}

    STATE_FUNC_DECLARE(dTgSparksTag, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgSparksTag);
};

#endif
