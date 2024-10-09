#ifndef D_A_E_SPARK_H
#define D_A_E_SPARK_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEspark_c : public dAcEnBase_c { 
public:
	dAcEspark_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEspark_c() {}

	STATE_FUNC_DECLARE(dAcEspark_c, Wait);
	STATE_FUNC_DECLARE(dAcEspark_c, EyeWait);
	STATE_FUNC_DECLARE(dAcEspark_c, Move);
	STATE_FUNC_DECLARE(dAcEspark_c, Damage);
	STATE_FUNC_DECLARE(dAcEspark_c, Dead);
	STATE_FUNC_DECLARE(dAcEspark_c, Stun);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEspark_c);
};

#endif
