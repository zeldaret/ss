#ifndef D_A_B_NUSI_H
#define D_A_B_NUSI_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNusi_c : public dAcEnBase_c {
public:
    dAcNusi_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNusi_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNusi_c);
};

#endif
