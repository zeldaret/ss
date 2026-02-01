#ifndef D_A_B_ASURA_FOOT_H
#define D_A_B_ASURA_FOOT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcAsuraFoot_c : public dAcObjBase_c {
public:
    dAcAsuraFoot_c() : mStateMgr(*this) {}
    virtual ~dAcAsuraFoot_c() {}

    STATE_FUNC_DECLARE(dAcAsuraFoot_c, Pulled);
    STATE_FUNC_DECLARE(dAcAsuraFoot_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcAsuraFoot_c);
};

#endif
