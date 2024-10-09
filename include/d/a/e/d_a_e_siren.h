#ifndef D_A_E_SIREN_H
#define D_A_E_SIREN_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcESiren_c : public dAcEnBase_c { 
public:
	dAcESiren_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcESiren_c() {}

	STATE_FUNC_DECLARE(dAcESiren_c, Wait);
	STATE_FUNC_DECLARE(dAcESiren_c, Revival);
	STATE_FUNC_DECLARE(dAcESiren_c, Move);
	STATE_FUNC_DECLARE(dAcESiren_c, PathMove);
	STATE_FUNC_DECLARE(dAcESiren_c, Turn);
	STATE_FUNC_DECLARE(dAcESiren_c, Event);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcESiren_c);
};
#endif