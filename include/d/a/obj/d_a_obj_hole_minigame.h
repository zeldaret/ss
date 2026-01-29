#ifndef D_A_OBJ_HOLE_MINIGAME_H
#define D_A_OBJ_HOLE_MINIGAME_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOholeMinigame_c : public dAcObjBase_c {
public:
    dAcOholeMinigame_c() : mStateMgr(*this) {}
    virtual ~dAcOholeMinigame_c() {}

    STATE_FUNC_DECLARE(dAcOholeMinigame_c, Wait);
    STATE_FUNC_DECLARE(dAcOholeMinigame_c, Start);
    STATE_FUNC_DECLARE(dAcOholeMinigame_c, Play);
    STATE_FUNC_DECLARE(dAcOholeMinigame_c, Message);
    STATE_FUNC_DECLARE(dAcOholeMinigame_c, Clear);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOholeMinigame_c);
};

#endif
