#ifndef D_A_E_PH_H
#define D_A_E_PH_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEPh_c : public dAcEnBase_c { 
public:
	dAcEPh_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEPh_c() {}

	STATE_FUNC_DECLARE(dAcEPh_c, Stop);
	STATE_FUNC_DECLARE(dAcEPh_c, PathMove);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEPh_c);
};
#endif