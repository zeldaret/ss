#ifndef D_A_OBJ_ASURA_PILLAR_H
#define D_A_OBJ_ASURA_PILLAR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcAsuraPillar_c : public dAcObjBase_c {
public:
    dAcAsuraPillar_c() : mStateMgr(*this) {}
    virtual ~dAcAsuraPillar_c() {}

    STATE_FUNC_DECLARE(dAcAsuraPillar_c, Damage);
    STATE_FUNC_DECLARE(dAcAsuraPillar_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcAsuraPillar_c);
};

#endif
