#ifndef D_A_E_OR_H
#define D_A_E_OR_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEor_c : public dAcEnBase_c { 
public:
	dAcEor_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEor_c() {}

	STATE_FUNC_DECLARE(dAcEor_c, Wait);
	STATE_FUNC_DECLARE(dAcEor_c, Appear);
	STATE_FUNC_DECLARE(dAcEor_c, Attack);
	STATE_FUNC_DECLARE(dAcEor_c, AttackCarry);
	STATE_FUNC_DECLARE(dAcEor_c, Stun);
	STATE_FUNC_DECLARE(dAcEor_c, Damage);
	STATE_FUNC_DECLARE(dAcEor_c, Hide);
	STATE_FUNC_DECLARE(dAcEor_c, Kyoro);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEor_c);
};
#endif