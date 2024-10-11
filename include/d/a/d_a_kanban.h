#ifndef D_A_KANBAN_H
#define D_A_KANBAN_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcKanban_c : public dAcObjBase_c {
public:
    dAcKanban_c();
    virtual ~dAcKanban_c();

    STATE_FUNC_DECLARE(dAcKanban_c, Wait);
    STATE_FUNC_DECLARE(dAcKanban_c, Message);
    STATE_FUNC_DECLARE(dAcKanban_c, PullOut);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcKanban_c);
};

#endif
