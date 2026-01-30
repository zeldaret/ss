#ifndef D_A_OBJ_KANBAN_STONE_H
#define D_A_OBJ_KANBAN_STONE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOKanbanStone_c : public dAcObjBase_c {
public:
    dAcOKanbanStone_c() : mStateMgr(*this) {}
    virtual ~dAcOKanbanStone_c() {}

    STATE_FUNC_DECLARE(dAcOKanbanStone_c, Wait);
    STATE_FUNC_DECLARE(dAcOKanbanStone_c, Message);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOKanbanStone_c);
};

#endif
