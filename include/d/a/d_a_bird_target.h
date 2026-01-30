#ifndef D_A_BIRD_TARGET_H
#define D_A_BIRD_TARGET_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dBird_Target_c : public dAcObjBase_c {
public:
    dBird_Target_c() : mStateMgr(*this) {}
    virtual ~dBird_Target_c() {}

    STATE_FUNC_DECLARE(dBird_Target_c, Wait);
    STATE_FUNC_DECLARE(dBird_Target_c, Move);
    STATE_FUNC_DECLARE(dBird_Target_c, LevelUpDemo);
    STATE_FUNC_DECLARE(dBird_Target_c, NpcWait);
    STATE_FUNC_DECLARE(dBird_Target_c, NpcGlide);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dBird_Target_c);
};

#endif
