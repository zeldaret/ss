#ifndef D_A_E_WS_H
#define D_A_E_WS_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEws_c : public dAcEnBase_c { 
public:
	dAcEws_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcEws_c() {}

	STATE_FUNC_DECLARE(dAcEws_c, Wait);
	STATE_FUNC_DECLARE(dAcEws_c, Move);
	STATE_FUNC_DECLARE(dAcEws_c, PyMove);
	STATE_FUNC_DECLARE(dAcEws_c, JumpReady);
	STATE_FUNC_DECLARE(dAcEws_c, JumpReady2);
	STATE_FUNC_DECLARE(dAcEws_c, Jump);
	STATE_FUNC_DECLARE(dAcEws_c, Hug);
	STATE_FUNC_DECLARE(dAcEws_c, ReturnJump);
	STATE_FUNC_DECLARE(dAcEws_c, Return);
	STATE_FUNC_DECLARE(dAcEws_c, Escape);
	STATE_FUNC_DECLARE(dAcEws_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcEws_c);
};

#endif
