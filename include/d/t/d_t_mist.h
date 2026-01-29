#ifndef D_T_MIST_H
#define D_T_MIST_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dMistTag_c : public dTg_c {
public:
    dMistTag_c() : mStateMgr(*this) {}
    virtual ~dMistTag_c() {}

    STATE_FUNC_DECLARE(dMistTag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dMistTag_c);
};

#endif
