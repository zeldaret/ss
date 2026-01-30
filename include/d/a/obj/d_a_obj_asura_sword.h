#ifndef D_A_OBJ_ASURA_SWORD_H
#define D_A_OBJ_ASURA_SWORD_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcAsuraSword_c : public dAcObjBase_c {
public:
    dAcAsuraSword_c() : mStateMgr(*this) {}
    virtual ~dAcAsuraSword_c() {}

    STATE_FUNC_DECLARE(dAcAsuraSword_c, Carried);
    STATE_FUNC_DECLARE(dAcAsuraSword_c, Stop);
    STATE_FUNC_DECLARE(dAcAsuraSword_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcAsuraSword_c);
};

#endif
