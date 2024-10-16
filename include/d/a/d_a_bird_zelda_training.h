#ifndef D_A_BIRD_ZELDA_TRAINING_H
#define D_A_BIRD_ZELDA_TRAINING_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dBird_ZeldaTraining_c : public dAcObjBase_c {
public:
    dBird_ZeldaTraining_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dBird_ZeldaTraining_c() {}

    STATE_FUNC_DECLARE(dBird_ZeldaTraining_c, Wait);
    STATE_FUNC_DECLARE(dBird_ZeldaTraining_c, Chase);
    STATE_FUNC_DECLARE(dBird_ZeldaTraining_c, Finish);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dBird_ZeldaTraining_c);
};

#endif
