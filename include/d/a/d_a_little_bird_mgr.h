#ifndef D_A_LITTLE_BIRD_MGR_H
#define D_A_LITTLE_BIRD_MGR_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"

class dAcLittleBirdMgr_c : public dAcBase_c {
public:
    dAcLittleBirdMgr_c() : mStateMgr(*this) {}
    virtual ~dAcLittleBirdMgr_c() {}

    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Set);
    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Reset);
    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Safety);
    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Warning);
    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Danger);
    STATE_FUNC_DECLARE(dAcLittleBirdMgr_c, Kill);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcLittleBirdMgr_c);
};

#endif
