#ifndef D_T_MIECHAN_H
#define D_T_MIECHAN_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgMiechan_c : public dTg_c {
public:
    dTgMiechan_c() : mStateMgr(*this) {}
    virtual ~dTgMiechan_c() {}

    STATE_FUNC_DECLARE(dTgMiechan_c, Search);
    STATE_FUNC_DECLARE(dTgMiechan_c, Wait1);
    STATE_FUNC_DECLARE(dTgMiechan_c, Wait2);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgMiechan_c);
};

#endif
