#ifndef D_A_E_MR_SHIELD_H
#define D_A_E_MR_SHIELD_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcE_Mr_Shield_c : public dAcEnBase_c {
public:
    dAcE_Mr_Shield_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcE_Mr_Shield_c() {}

    STATE_FUNC_DECLARE(dAcE_Mr_Shield_c, Move);
    STATE_FUNC_DECLARE(dAcE_Mr_Shield_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcE_Mr_Shield_c);
};

#endif
