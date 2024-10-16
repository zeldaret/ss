#ifndef D_A_E_MR_H
#define D_A_E_MR_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEmr_c : public dAcEnBase_c {
public:
    dAcEmr_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEmr_c() {}

    STATE_FUNC_DECLARE(dAcEmr_c, Wait);
    STATE_FUNC_DECLARE(dAcEmr_c, Walk);
    STATE_FUNC_DECLARE(dAcEmr_c, Find);
    STATE_FUNC_DECLARE(dAcEmr_c, BattleMove);
    STATE_FUNC_DECLARE(dAcEmr_c, GuardSt);
    STATE_FUNC_DECLARE(dAcEmr_c, GuardEnd);
    STATE_FUNC_DECLARE(dAcEmr_c, Stick);
    STATE_FUNC_DECLARE(dAcEmr_c, BattleNoShieldMove);
    STATE_FUNC_DECLARE(dAcEmr_c, Swing);
    STATE_FUNC_DECLARE(dAcEmr_c, Rush);
    STATE_FUNC_DECLARE(dAcEmr_c, GuardJust);
    STATE_FUNC_DECLARE(dAcEmr_c, Lost);
    STATE_FUNC_DECLARE(dAcEmr_c, ShieldCut);
    STATE_FUNC_DECLARE(dAcEmr_c, ShieldThrow);
    STATE_FUNC_DECLARE(dAcEmr_c, ShieldOut);
    STATE_FUNC_DECLARE(dAcEmr_c, Dead);
    STATE_FUNC_DECLARE(dAcEmr_c, Piyori);
    STATE_FUNC_DECLARE(dAcEmr_c, WhipPiyori);
    STATE_FUNC_DECLARE(dAcEmr_c, BombReaction);
    STATE_FUNC_DECLARE(dAcEmr_c, Yoroke);
    STATE_FUNC_DECLARE(dAcEmr_c, EventWait);
    STATE_FUNC_DECLARE(dAcEmr_c, Damage);
    STATE_FUNC_DECLARE(dAcEmr_c, PathWait);
    STATE_FUNC_DECLARE(dAcEmr_c, PathWalk);
    STATE_FUNC_DECLARE(dAcEmr_c, Home);
    STATE_FUNC_DECLARE(dAcEmr_c, PathHome);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEmr_c);
};

#endif
