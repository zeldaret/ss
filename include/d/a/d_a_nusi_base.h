#ifndef D_A_NUSI_BASE_H
#define D_A_NUSI_BASE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNusiBase_c : public dAcEnBase_c { 
public:
	dAcNusiBase_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNusiBase_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNusiBase_c);
};
#endif