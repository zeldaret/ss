#ifndef D_A_NUSI_NPC_H
#define D_A_NUSI_NPC_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNusiNpc_c : public dAcEnBase_c { 
public:
	dAcNusiNpc_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNusiNpc_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNusiNpc_c);
};

#endif
