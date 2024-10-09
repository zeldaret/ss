#ifndef D_A_E_BCZ_H
#define D_A_E_BCZ_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEBcZ_c : public dAcEnBase_c { 
public:
	dAcEBcZ_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEBcZ_c() {}

	STATE_FUNC_DECLARE(dAcEBcZ_c, Arise);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Wait);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Walk);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Find);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Chase);
	STATE_FUNC_DECLARE(dAcEBcZ_c, HoldAttack);
	STATE_FUNC_DECLARE(dAcEBcZ_c, HoldStart);
	STATE_FUNC_DECLARE(dAcEBcZ_c, HoldWait);
	STATE_FUNC_DECLARE(dAcEBcZ_c, HoldScream);
	STATE_FUNC_DECLARE(dAcEBcZ_c, WhipTurn);
	STATE_FUNC_DECLARE(dAcEBcZ_c, WhipStagger);
	STATE_FUNC_DECLARE(dAcEBcZ_c, WindBlow);
	STATE_FUNC_DECLARE(dAcEBcZ_c, KeepAwayFromShield);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Damage);
	STATE_FUNC_DECLARE(dAcEBcZ_c, Down);
	STATE_FUNC_DECLARE(dAcEBcZ_c, AriseFromTag);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeSearch);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeSearchWalk);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeSearchWait);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeWait);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeMove);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeHoldStart);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeHoldWait);
	STATE_FUNC_DECLARE(dAcEBcZ_c, RopeHoldScream);
	STATE_FUNC_DECLARE(dAcEBcZ_c, PoisonSwamp);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEBcZ_c);
};

#endif
