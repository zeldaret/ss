#ifndef D_T_KYTAG_H
#define D_T_KYTAG_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dKytagTag_c : public dAcBase_c { 
public:
	dKytagTag_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dKytagTag_c() {}

	STATE_FUNC_DECLARE(dKytagTag_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dKytagTag_c);
};
#endif