#ifndef D_A_INSECT_SCARAB_H
#define D_A_INSECT_SCARAB_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcInsectScarab_c : public dAcObjBase_c {
public:
    dAcInsectScarab_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcInsectScarab_c() {}

    STATE_FUNC_DECLARE(dAcInsectScarab_c, Wait);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Walk);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Excrement);
    STATE_FUNC_DECLARE(dAcInsectScarab_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectScarab_c);
};

#endif
