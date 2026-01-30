#ifndef D_A_INSECT_ANT_H
#define D_A_INSECT_ANT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcInsectAnt_c : public dAcObjBase_c {
public:
    dAcInsectAnt_c() : mStateMgr(*this) {}
    virtual ~dAcInsectAnt_c() {}

    STATE_FUNC_DECLARE(dAcInsectAnt_c, Wait);
    STATE_FUNC_DECLARE(dAcInsectAnt_c, Walk);
    STATE_FUNC_DECLARE(dAcInsectAnt_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectAnt_c, Dig);
    STATE_FUNC_DECLARE(dAcInsectAnt_c, Dead);
    STATE_FUNC_DECLARE(dAcInsectAnt_c, Finalize);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectAnt_c);
};

#endif
