#ifndef D_A_E_SKYTAIL_H
#define D_A_E_SKYTAIL_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcESkytail_c : public dAcEnBase_c { 
public:
	dAcESkytail_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcESkytail_c() {}

	STATE_FUNC_DECLARE(dAcESkytail_c, RandomMove);
	STATE_FUNC_DECLARE(dAcESkytail_c, Move);
	STATE_FUNC_DECLARE(dAcESkytail_c, ParallelMove);
	STATE_FUNC_DECLARE(dAcESkytail_c, FrontAttack);
	STATE_FUNC_DECLARE(dAcESkytail_c, Charge);
	STATE_FUNC_DECLARE(dAcESkytail_c, Dead);
	STATE_FUNC_DECLARE(dAcESkytail_c, Skytail);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcESkytail_c);
};

#endif
