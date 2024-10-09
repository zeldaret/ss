#ifndef D_A_B_ASURA_H
#define D_A_B_ASURA_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBAsura_c : public dAcEnBase_c { 
public:
	dAcBAsura_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcBAsura_c() {}

	STATE_FUNC_DECLARE(dAcBAsura_c, Attack);
	STATE_FUNC_DECLARE(dAcBAsura_c, AttackHigh);
	STATE_FUNC_DECLARE(dAcBAsura_c, AttackB);
	STATE_FUNC_DECLARE(dAcBAsura_c, BreakLegB);
	STATE_FUNC_DECLARE(dAcBAsura_c, BreakWaitB);
	STATE_FUNC_DECLARE(dAcBAsura_c, ChargeB);
	STATE_FUNC_DECLARE(dAcBAsura_c, CutAllB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Damage);
	STATE_FUNC_DECLARE(dAcBAsura_c, DamageB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Dead);
	STATE_FUNC_DECLARE(dAcBAsura_c, DeadB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Disconnect);
	STATE_FUNC_DECLARE(dAcBAsura_c, DisconnectB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Fire);
	STATE_FUNC_DECLARE(dAcBAsura_c, Recover);
	STATE_FUNC_DECLARE(dAcBAsura_c, RecoverB);
	STATE_FUNC_DECLARE(dAcBAsura_c, StunB);
	STATE_FUNC_DECLARE(dAcBAsura_c, SummonB);
	STATE_FUNC_DECLARE(dAcBAsura_c, SwordGame);
	STATE_FUNC_DECLARE(dAcBAsura_c, Wait);
	STATE_FUNC_DECLARE(dAcBAsura_c, WaitB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Start);
	STATE_FUNC_DECLARE(dAcBAsura_c, StartB);
	STATE_FUNC_DECLARE(dAcBAsura_c, Test);
	STATE_FUNC_DECLARE(dAcBAsura_c, TestB);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcBAsura_c);
};
#endif