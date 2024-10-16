#ifndef D_A_B_GIRAHIMU2_H
#define D_A_B_GIRAHIMU2_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcGirahimu2_c : public dAcEnBase_c {
public:
    dAcGirahimu2_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimu2_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu2_c);
};

#endif
