#ifndef D_A_E_BCE_H
#define D_A_E_BCE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbce_c : public dAcEnBase_c { 
public:
	dAcEbce_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEbce_c() {}

	STATE_FUNC_DECLARE(dAcEbce_c, Standby);
	STATE_FUNC_DECLARE(dAcEbce_c, Home);
	STATE_FUNC_DECLARE(dAcEbce_c, Normal);
	STATE_FUNC_DECLARE(dAcEbce_c, Path);
	STATE_FUNC_DECLARE(dAcEbce_c, FightStart);
	STATE_FUNC_DECLARE(dAcEbce_c, Fight);
	STATE_FUNC_DECLARE(dAcEbce_c, Attack);
	STATE_FUNC_DECLARE(dAcEbce_c, StepAttack);
	STATE_FUNC_DECLARE(dAcEbce_c, Damage);
	STATE_FUNC_DECLARE(dAcEbce_c, AirDamage);
	STATE_FUNC_DECLARE(dAcEbce_c, Fail);
	STATE_FUNC_DECLARE(dAcEbce_c, BombAction);
	STATE_FUNC_DECLARE(dAcEbce_c, StabGuard);
	STATE_FUNC_DECLARE(dAcEbce_c, Stun);
	STATE_FUNC_DECLARE(dAcEbce_c, Drop);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEbce_c);
};

#endif
