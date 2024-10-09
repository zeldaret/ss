#ifndef D_T_INSECT_H
#define D_T_INSECT_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgInsect_c : public dAcBase_c { 
public:
	dTgInsect_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgInsect_c() {}

	STATE_FUNC_DECLARE(dTgInsect_c, Wait);
	STATE_FUNC_DECLARE(dTgInsect_c, WaitCreate);
	STATE_FUNC_DECLARE(dTgInsect_c, WaitForceEscape);
	STATE_FUNC_DECLARE(dTgInsect_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgInsect_c);
};
#endif