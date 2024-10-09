#ifndef D_A_E_HB_H
#define D_A_E_HB_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEhb_c : public dAcEnBase_c { 
public:
	dAcEhb_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEhb_c() {}

	STATE_FUNC_DECLARE(dAcEhb_c, Under);
	STATE_FUNC_DECLARE(dAcEhb_c, Appear);
	STATE_FUNC_DECLARE(dAcEhb_c, Wait);
	STATE_FUNC_DECLARE(dAcEhb_c, Attack);
	STATE_FUNC_DECLARE(dAcEhb_c, Damage);
	STATE_FUNC_DECLARE(dAcEhb_c, Blow);
	STATE_FUNC_DECLARE(dAcEhb_c, Stun);
	STATE_FUNC_DECLARE(dAcEhb_c, DeadMouth);
	STATE_FUNC_DECLARE(dAcEhb_c, Avoid);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEhb_c);
};

#endif
