#ifndef D_A_E_GEROCK_H
#define D_A_E_GEROCK_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEgerock_c : public dAcEnBase_c { 
public:
	dAcEgerock_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEgerock_c() {}

	STATE_FUNC_DECLARE(dAcEgerock_c, Attack);
	STATE_FUNC_DECLARE(dAcEgerock_c, Damage);
	STATE_FUNC_DECLARE(dAcEgerock_c, FlyHome);
	STATE_FUNC_DECLARE(dAcEgerock_c, Rail);
	STATE_FUNC_DECLARE(dAcEgerock_c, Return);
	STATE_FUNC_DECLARE(dAcEgerock_c, Shock);
	STATE_FUNC_DECLARE(dAcEgerock_c, Stay);
	STATE_FUNC_DECLARE(dAcEgerock_c, Vanish);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEgerock_c);
};
#endif