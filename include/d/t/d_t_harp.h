#ifndef D_T_HARP_H
#define D_T_HARP_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgHarp_c : public dTg_c {
public:
    dTgHarp_c() : mStateMgr(*this) {}
    virtual ~dTgHarp_c() {}

    STATE_FUNC_DECLARE(dTgHarp_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgHarp_c);
};

#endif
