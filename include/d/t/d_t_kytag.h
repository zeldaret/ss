#ifndef D_T_KYTAG_H
#define D_T_KYTAG_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dKytagTag_c : public dTg_c {
public:
    dKytagTag_c() : mStateMgr(*this) {}
    virtual ~dKytagTag_c() {}

    STATE_FUNC_DECLARE(dKytagTag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dKytagTag_c);
};

#endif
