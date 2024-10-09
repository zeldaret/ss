#ifndef D_A_E_BCAL_ARROW_H
#define D_A_E_BCAL_ARROW_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbcalarrow_c : public dAcEnBase_c { 
public:
	dAcEbcalarrow_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEbcalarrow_c() {}

	STATE_FUNC_DECLARE(dAcEbcalarrow_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEbcalarrow_c);
};
#endif