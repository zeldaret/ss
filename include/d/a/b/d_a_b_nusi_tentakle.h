#ifndef D_A_B_NUSI_TENTAKLE_H
#define D_A_B_NUSI_TENTAKLE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNusiTentakle_c : public dAcEnBase_c { 
public:
	dAcNusiTentakle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNusiTentakle_c() {}

	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Hide);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Appear);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Wait);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, HideWait);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Attack);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, AppearDemo);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, HideAppearDemo);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, ScarfDamage);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Anger);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Down);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, DownDamage);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, LastDamage);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Recover);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, Death);
	STATE_FUNC_DECLARE(dAcNusiTentakle_c, CloseEye);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNusiTentakle_c);
};

#endif
