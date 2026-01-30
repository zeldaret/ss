#ifndef D_A_OBJ_LOTUS_SEED_H
#define D_A_OBJ_LOTUS_SEED_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOLotusSeed_c : public dAcObjBase_c {
public:
    dAcOLotusSeed_c() : mStateMgr(*this) {}
    virtual ~dAcOLotusSeed_c() {}

    STATE_FUNC_DECLARE(dAcOLotusSeed_c, Init);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, Stick);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, Wait);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, CarryGrab);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, CarryWhip);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, CarryBeetle);
    STATE_FUNC_DECLARE(dAcOLotusSeed_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLotusSeed_c);
};

#endif
