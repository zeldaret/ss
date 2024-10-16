#ifndef D_A_E_HIDOKARIS_H
#define D_A_E_HIDOKARIS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEhidokaris_c : public dAcObjBase_c {
public:
    dAcEhidokaris_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEhidokaris_c() {}

    STATE_FUNC_DECLARE(dAcEhidokaris_c, Wait);
    STATE_FUNC_DECLARE(dAcEhidokaris_c, BombWait);
    STATE_FUNC_DECLARE(dAcEhidokaris_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEhidokaris_c);
};

#endif
