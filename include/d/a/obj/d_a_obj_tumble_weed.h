#ifndef D_A_OBJ_TUMBLE_WEED_H
#define D_A_OBJ_TUMBLE_WEED_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTumbleWeed_c : public dAcObjBase_c {
public:
    dAcOTumbleWeed_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTumbleWeed_c() {}

    STATE_FUNC_DECLARE(dAcOTumbleWeed_c, Wait);
    STATE_FUNC_DECLARE(dAcOTumbleWeed_c, Slope);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTumbleWeed_c);
};

#endif
