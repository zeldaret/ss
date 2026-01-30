#ifndef D_T_BB_QUAKE_H
#define D_T_BB_QUAKE_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgBBQuake_c : public dTg_c {
public:
    dTgBBQuake_c() : mStateMgr(*this) {}
    virtual ~dTgBBQuake_c() {}

    STATE_FUNC_DECLARE(dTgBBQuake_c, Wait);
    STATE_FUNC_DECLARE(dTgBBQuake_c, Wake);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgBBQuake_c);
};

#endif
