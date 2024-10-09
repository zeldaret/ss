#ifndef D_T_SIREN_TIME_ATTACK_H
#define D_T_SIREN_TIME_ATTACK_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgSirenTimeAttack_c : public dAcBase_c { 
public:
	dTgSirenTimeAttack_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgSirenTimeAttack_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgSirenTimeAttack_c);
};

#endif
