#ifndef D_A_E_MAGUPPO_H
#define D_A_E_MAGUPPO_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEmaguppo_c : public dAcEnBase_c { 
public:
	dAcEmaguppo_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEmaguppo_c() {}

	STATE_FUNC_DECLARE(dAcEmaguppo_c, Attack);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Appear);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Blow);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Corner);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Damage);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Dead);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Dive);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, RailReverse);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, RailWalk);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Stomped);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, Stun);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, DivingWait);
	STATE_FUNC_DECLARE(dAcEmaguppo_c, WaitAttack);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEmaguppo_c);
};

#endif
