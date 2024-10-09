#ifndef D_A_E_BC_ARROW_H
#define D_A_E_BC_ARROW_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEbcarrow_c : public dAcEnBase_c { 
public:
	dAcEbcarrow_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEbcarrow_c() {}

	STATE_FUNC_DECLARE(dAcEbcarrow_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEbcarrow_c);
};

#endif
