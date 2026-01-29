#ifndef D_T_KS_H
#define D_T_KS_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgKiesuTag_c : public dTg_c {
public:
    dTgKiesuTag_c() : mStateMgr(*this) {}
    virtual ~dTgKiesuTag_c() {}

    STATE_FUNC_DECLARE(dTgKiesuTag_c, CreateChildren);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlWait);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlReturnToWait);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlMove);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlFighting);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, ControlPathMove);
    STATE_FUNC_DECLARE(dTgKiesuTag_c, DoNothing);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgKiesuTag_c);
};

#endif
