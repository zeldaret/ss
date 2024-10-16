#ifndef D_A_OBJ_UTAJIMA_PEDESTAL_H
#define D_A_OBJ_UTAJIMA_PEDESTAL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOutajimaPedestal_c : public dAcObjBase_c {
public:
    dAcOutajimaPedestal_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOutajimaPedestal_c() {}

    STATE_FUNC_DECLARE(dAcOutajimaPedestal_c, Wait);
    STATE_FUNC_DECLARE(dAcOutajimaPedestal_c, Move);
    STATE_FUNC_DECLARE(dAcOutajimaPedestal_c, Reset);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOutajimaPedestal_c);
};

#endif
