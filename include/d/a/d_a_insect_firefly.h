#ifndef D_A_INSECT_FIREFLY_H
#define D_A_INSECT_FIREFLY_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcInsectFirefly_c : public dAcObjBase_c {
public:
    dAcInsectFirefly_c() : mStateMgr(*this) {}
    virtual ~dAcInsectFirefly_c() {}

    STATE_FUNC_DECLARE(dAcInsectFirefly_c, Move);
    STATE_FUNC_DECLARE(dAcInsectFirefly_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectFirefly_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectFirefly_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectFirefly_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectFirefly_c);
};

#endif
