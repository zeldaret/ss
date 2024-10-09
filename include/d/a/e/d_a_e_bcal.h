#ifndef D_A_E_BCAL_H
#define D_A_E_BCAL_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbcaL_c : public dAcEnBase_c { 
public:
	dAcEbcaL_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEbcaL_c() {}

	STATE_FUNC_DECLARE(dAcEbcaL_c, Standby);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Attack);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Damage);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Hit);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Lead);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Stun);
	STATE_FUNC_DECLARE(dAcEbcaL_c, AirDamage);
	STATE_FUNC_DECLARE(dAcEbcaL_c, Fail);
	STATE_FUNC_DECLARE(dAcEbcaL_c, StabGuard);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEbcaL_c);
};

#endif
