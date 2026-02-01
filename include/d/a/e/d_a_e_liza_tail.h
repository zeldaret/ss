#ifndef D_A_E_LIZA_TAIL_H
#define D_A_E_LIZA_TAIL_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEliza_tail_c : public dAcEnBase_c {
public:
    dAcEliza_tail_c() : mStateMgr(*this) {}
    virtual ~dAcEliza_tail_c() {}

    STATE_FUNC_DECLARE(dAcEliza_tail_c, Wait);
    STATE_FUNC_DECLARE(dAcEliza_tail_c, Damage);
    STATE_FUNC_DECLARE(dAcEliza_tail_c, WhipCatch);
    STATE_FUNC_DECLARE(dAcEliza_tail_c, Get);
    STATE_FUNC_DECLARE(dAcEliza_tail_c, InLava);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEliza_tail_c);
};

#endif
