#ifndef D_A_OBJ_ROULETTE_ISLAND_C_H
#define D_A_OBJ_ROULETTE_ISLAND_C_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOrouletteIslandC_c : public dAcObjBase_c {
public:
    dAcOrouletteIslandC_c();
    virtual ~dAcOrouletteIslandC_c();

    STATE_FUNC_DECLARE(dAcOrouletteIslandC_c, Wait);
    STATE_FUNC_DECLARE(dAcOrouletteIslandC_c, Play);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOrouletteIslandC_c);
};

#endif
