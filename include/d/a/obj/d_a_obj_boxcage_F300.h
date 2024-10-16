#ifndef D_A_OBJ_BOXCAGE_F300_H
#define D_A_OBJ_BOXCAGE_F300_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOBoxCageF300_c : public dAcObjBase_c {
public:
    dAcOBoxCageF300_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBoxCageF300_c() {}

    STATE_FUNC_DECLARE(dAcOBoxCageF300_c, Wait);
    STATE_FUNC_DECLARE(dAcOBoxCageF300_c, Move);
    STATE_FUNC_DECLARE(dAcOBoxCageF300_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBoxCageF300_c);
};

#endif
