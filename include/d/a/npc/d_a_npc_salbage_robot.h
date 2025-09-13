#ifndef D_A_NPC_SALBAGE_ROBOT_H
#define D_A_NPC_SALBAGE_ROBOT_H

#include "d/a/npc/d_a_ordinary_npc.h"
#include "m/m_mtx.h"
#include "s/s_State.hpp"

class dAcNpcSlb_c : public dAcOrdinaryNpc_c {
public:
    dAcNpcSlb_c() {}
    virtual ~dAcNpcSlb_c() {}

    STATE_FUNC_DECLARE(dAcNpcSlb_c, Hide);
    STATE_FUNC_DECLARE(dAcNpcSlb_c, HideDelivary);
    STATE_FUNC_DECLARE(dAcNpcSlb_c, DelivaryReq);
    STATE_FUNC_DECLARE(dAcNpcSlb_c, Kill);
    STATE_FUNC_DECLARE(dAcNpcSlb_c, Kill2);

    // ouch
    mMtx_c getCarriedObjMtx() const {
        return mCarryObjMtx;
    }

private:
    u8 _pad[0xD50 - sizeof(dAcOrdinaryNpc_c)];

    /* 0xD50 */ mMtx_c mCarryObjMtx;
};

#endif
