#ifndef D_A_OBJ_FRUIT_TREE_H
#define D_A_OBJ_FRUIT_TREE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFruitTree_c : public dAcObjBase_c {
public:
    dAcOFruitTree_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOFruitTree_c() {}

    STATE_FUNC_DECLARE(dAcOFruitTree_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOFruitTree_c);
};

#endif
