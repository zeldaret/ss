#ifndef D_A_INSECT_LADYBUG_H
#define D_A_INSECT_LADYBUG_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcInsectLadybug_c : public dAcObjBase_c {
public:
    dAcInsectLadybug_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcInsectLadybug_c() {}

    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Wait);
    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Walk);
    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectLadybug_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectLadybug_c);
};

#endif
