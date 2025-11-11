#ifndef D_T_HARP_H
#define D_T_HARP_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgHarp_c : public dAcBase_c {
public:
    dTgHarp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgHarp_c() {}

    STATE_FUNC_DECLARE(dTgHarp_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgHarp_c);
    short fn_536_530();
    unsigned short fn_536_690(unsigned short *param_1);
};

#endif
