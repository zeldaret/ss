#ifndef D_A_OBJ_ITEM_HEART_CONTAINER_H
#define D_A_OBJ_ITEM_HEART_CONTAINER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOItemHeartContainer_c : public dAcObjBase_c {
public:
    dAcOItemHeartContainer_c();
    virtual ~dAcOItemHeartContainer_c();

    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, Hide);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, RequestAppearEvent);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, AppearEffect);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, Appear);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, WaitFall);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, Fall);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, WaitGet);
    STATE_FUNC_DECLARE(dAcOItemHeartContainer_c, WaitGetQuake);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOItemHeartContainer_c);
};

#endif
