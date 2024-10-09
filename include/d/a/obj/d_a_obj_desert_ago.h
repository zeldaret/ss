#ifndef D_A_OBJ_DESERT_AGO_H
#define D_A_OBJ_DESERT_AGO_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODesertAgo_c : public dAcObjBase_c {
public:
    dAcODesertAgo_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcODesertAgo_c() {}

    STATE_FUNC_DECLARE(dAcODesertAgo_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcODesertAgo_c);
};

#endif
