#ifndef D_T_GEKOTAG_H
#define D_T_GEKOTAG_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgGekoTag : public dAcBase_c { 
public:
	dTgGekoTag() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgGekoTag() {}

	STATE_FUNC_DECLARE(dTgGekoTag, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgGekoTag);
};

#endif
