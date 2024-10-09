#ifndef D_T_BB_QUAKE_H
#define D_T_BB_QUAKE_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgBBQuake_c : public dAcBase_c { 
public:
	dTgBBQuake_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgBBQuake_c() {}

	STATE_FUNC_DECLARE(dTgBBQuake_c, Wait);
	STATE_FUNC_DECLARE(dTgBBQuake_c, Wake);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgBBQuake_c);
};

#endif
