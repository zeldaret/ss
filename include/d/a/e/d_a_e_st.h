#ifndef D_A_E_ST_H
#define D_A_E_ST_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEst_c : public dAcEnBase_c {
public:
    dAcEst_c() : mStateMgr(*this) {}
    virtual ~dAcEst_c() {}

    STATE_FUNC_DECLARE(dAcEst_c, Home);
    STATE_FUNC_DECLARE(dAcEst_c, HomeLine);
    STATE_FUNC_DECLARE(dAcEst_c, HangHide);
    STATE_FUNC_DECLARE(dAcEst_c, HangWait);
    STATE_FUNC_DECLARE(dAcEst_c, HangAttack);
    STATE_FUNC_DECLARE(dAcEst_c, HangStun);
    STATE_FUNC_DECLARE(dAcEst_c, Drop);
    STATE_FUNC_DECLARE(dAcEst_c, NormalWalk);
    STATE_FUNC_DECLARE(dAcEst_c, NormalWait);
    STATE_FUNC_DECLARE(dAcEst_c, Fight);
    STATE_FUNC_DECLARE(dAcEst_c, JumpAttack);
    STATE_FUNC_DECLARE(dAcEst_c, WireAttack);
    STATE_FUNC_DECLARE(dAcEst_c, Hug);
    STATE_FUNC_DECLARE(dAcEst_c, HugWalk);
    STATE_FUNC_DECLARE(dAcEst_c, CancelHug);
    STATE_FUNC_DECLARE(dAcEst_c, Stun);
    STATE_FUNC_DECLARE(dAcEst_c, StandEnd);
    STATE_FUNC_DECLARE(dAcEst_c, StandStun);
    STATE_FUNC_DECLARE(dAcEst_c, StandHit);
    STATE_FUNC_DECLARE(dAcEst_c, Defense);
    STATE_FUNC_DECLARE(dAcEst_c, Dead);
    STATE_FUNC_DECLARE(dAcEst_c, WaterDead);
    STATE_FUNC_DECLARE(dAcEst_c, StandDead);
    STATE_FUNC_DECLARE(dAcEst_c, Reverse);
    STATE_FUNC_DECLARE(dAcEst_c, SpiderWait);
    STATE_FUNC_DECLARE(dAcEst_c, SpiderMove);
    STATE_FUNC_DECLARE(dAcEst_c, SpiderAttack);
    STATE_FUNC_DECLARE(dAcEst_c, SpiderHome);
    STATE_FUNC_DECLARE(dAcEst_c, SpiderFang);
    STATE_FUNC_DECLARE(dAcEst_c, WindGuard);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEst_c);
};

#endif
