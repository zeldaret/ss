#ifndef D_T_SPARKS_H
#define D_T_SPARKS_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgSparksTag : public dTg_c {
public:
    dTgSparksTag() : mStateMgr(*this) {}
    virtual ~dTgSparksTag() {}

    STATE_FUNC_DECLARE(dTgSparksTag, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgSparksTag);
};

#endif
