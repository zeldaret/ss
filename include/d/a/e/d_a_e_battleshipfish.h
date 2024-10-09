#ifndef D_A_E_BATTLESHIPFISH_H
#define D_A_E_BATTLESHIPFISH_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbfish_c : public dAcEnBase_c { 
public:
	dAcEbfish_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEbfish_c() {}

	STATE_FUNC_DECLARE(dAcEbfish_c, Wait);
	STATE_FUNC_DECLARE(dAcEbfish_c, Move);
	STATE_FUNC_DECLARE(dAcEbfish_c, Find);
	STATE_FUNC_DECLARE(dAcEbfish_c, Attack);
	STATE_FUNC_DECLARE(dAcEbfish_c, Piyo);
	STATE_FUNC_DECLARE(dAcEbfish_c, NoDamage);
	STATE_FUNC_DECLARE(dAcEbfish_c, Dead);
	STATE_FUNC_DECLARE(dAcEbfish_c, BirthWait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEbfish_c);
};
#endif