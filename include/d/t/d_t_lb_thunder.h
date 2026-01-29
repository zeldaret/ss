#ifndef D_T_LB_THUNDER_H
#define D_T_LB_THUNDER_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgLbThunder_c : public dTg_c {
public:
    dTgLbThunder_c() : mStateMgr(*this) {}
    virtual ~dTgLbThunder_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgLbThunder_c);
};

#endif
