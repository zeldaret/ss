#ifndef D_A_B_GIRAHIMU_H
#define D_A_B_GIRAHIMU_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcGirahimu_c : public dAcEnBase_c {
public:
    dAcGirahimu_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimu_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu_c);
};

#endif
