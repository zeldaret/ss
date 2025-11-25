#ifndef D_A_OBJ_FENCE_H
#define D_A_OBJ_FENCE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOFence_c : public dAcObjBase_c {
public:
    dAcOFence_c();
    virtual ~dAcOFence_c();

    bool fn_550_11B0() const;
    void fn_550_12C0();

    void changeToRequestConfineEvent() {
        mStateMgr.changeState(StateID_RequestConfineEvent);
    }

    STATE_FUNC_DECLARE(dAcOFence_c, WaitOpen);
    STATE_FUNC_DECLARE(dAcOFence_c, Open);
    STATE_FUNC_DECLARE(dAcOFence_c, OpenPocoAPoco);
    STATE_FUNC_DECLARE(dAcOFence_c, SuccessPocoAPoco);
    STATE_FUNC_DECLARE(dAcOFence_c, StopOpenPocoAPoco);
    STATE_FUNC_DECLARE(dAcOFence_c, StopOpenAfterglowPocoAPoco);
    STATE_FUNC_DECLARE(dAcOFence_c, WaitClose);
    STATE_FUNC_DECLARE(dAcOFence_c, Close);
    STATE_FUNC_DECLARE(dAcOFence_c, RequestConfineEvent);

private:
    /* 0x330 */ u8 _0x330[0x368 - 0x330];
    /* 0x368 */ STATE_MGR_DECLARE(dAcOFence_c);
};

#endif
