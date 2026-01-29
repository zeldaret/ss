#ifndef D_A_OBJ_KIBAKO_H
#define D_A_OBJ_KIBAKO_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOkibako_c : public dAcObjBase_c {
public:
    dAcOkibako_c() : mStateMgr(*this) {}
    virtual ~dAcOkibako_c() {}

    STATE_FUNC_DECLARE(dAcOkibako_c, Wait);
    STATE_FUNC_DECLARE(dAcOkibako_c, Fire);
    STATE_FUNC_DECLARE(dAcOkibako_c, PushPull);
    STATE_FUNC_DECLARE(dAcOkibako_c, RopeGrab);
    STATE_FUNC_DECLARE(dAcOkibako_c, DeleteEvent);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOkibako_c);
};

#endif
