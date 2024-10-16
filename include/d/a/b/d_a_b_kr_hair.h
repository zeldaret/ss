#ifndef D_A_B_KR_HAIR_H
#define D_A_B_KR_HAIR_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcBkrh_c : public dAcEnBase_c {
public:
    dAcBkrh_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBkrh_c() {}

    STATE_FUNC_DECLARE(dAcBkrh_c, Normal);
    STATE_FUNC_DECLARE(dAcBkrh_c, Attack);
    STATE_FUNC_DECLARE(dAcBkrh_c, Escape);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBkrh_c);
};

#endif
