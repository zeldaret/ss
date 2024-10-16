#ifndef D_A_OBJ_SLICE_LOG_PARTS_H
#define D_A_OBJ_SLICE_LOG_PARTS_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOsliceLogParts_c : public dAcEnBase_c {
public:
    dAcOsliceLogParts_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsliceLogParts_c() {}

    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Wait);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Fly);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Ground);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, GroundSecond);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Shake);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Vanish);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, Finish);
    STATE_FUNC_DECLARE(dAcOsliceLogParts_c, GroundAftFin);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOsliceLogParts_c);
};

#endif
