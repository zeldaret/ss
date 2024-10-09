#ifndef D_T_SPORE_H
#define D_T_SPORE_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dSporeTag_c : public dAcBase_c { 
public:
	dSporeTag_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dSporeTag_c() {}

	STATE_FUNC_DECLARE(dSporeTag_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dSporeTag_c);
};
#endif