#ifndef D_A_OBJ_LAVA_D201_H
#define D_A_OBJ_LAVA_D201_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOLavaD201_c : public dAcObjBase_c {
public:
    dAcOLavaD201_c() : mStateMgr(*this) {}
    virtual ~dAcOLavaD201_c() {}

    STATE_FUNC_DECLARE(dAcOLavaD201_c, Wait);
    STATE_FUNC_DECLARE(dAcOLavaD201_c, WaitOff);
    STATE_FUNC_DECLARE(dAcOLavaD201_c, LavaIn);
    STATE_FUNC_DECLARE(dAcOLavaD201_c, LavaOut);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLavaD201_c);
};

#endif
