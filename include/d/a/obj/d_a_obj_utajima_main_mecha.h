#ifndef D_A_OBJ_UTAJIMA_MAIN_MECHA_H
#define D_A_OBJ_UTAJIMA_MAIN_MECHA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOutajimaMainMecha_c : public dAcObjBase_c {
public:
    dAcOutajimaMainMecha_c() : mStateMgr(*this) {}
    virtual ~dAcOutajimaMainMecha_c() {}

    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Before);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Appear);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Main);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Roll);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Disappear);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, After);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, StopperReset);
    STATE_FUNC_DECLARE(dAcOutajimaMainMecha_c, Reset);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOutajimaMainMecha_c);
};

#endif
