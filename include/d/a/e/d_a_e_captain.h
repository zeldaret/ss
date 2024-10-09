#ifndef D_A_E_CAPTAIN_H
#define D_A_E_CAPTAIN_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEcaptain_c : public dAcEnBase_c { 
public:
	dAcEcaptain_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEcaptain_c() {}

	STATE_FUNC_DECLARE(dAcEcaptain_c, Opening);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Wait);
	STATE_FUNC_DECLARE(dAcEcaptain_c, BalanceWait);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Walk);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Run);
	STATE_FUNC_DECLARE(dAcEcaptain_c, AttackSW);
	STATE_FUNC_DECLARE(dAcEcaptain_c, AttackF);
	STATE_FUNC_DECLARE(dAcEcaptain_c, AttackSP);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Guard);
	STATE_FUNC_DECLARE(dAcEcaptain_c, GuardJust);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Damage);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Dead);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Demo1);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Demo2);
	STATE_FUNC_DECLARE(dAcEcaptain_c, OpeningType2);
	STATE_FUNC_DECLARE(dAcEcaptain_c, DeadType2);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Demo1Type2);
	STATE_FUNC_DECLARE(dAcEcaptain_c, Demo2Type2);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEcaptain_c);
};
#endif