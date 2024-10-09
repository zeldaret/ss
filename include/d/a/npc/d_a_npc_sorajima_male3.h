#ifndef D_A_NPC_SORAJIMA_MALE3_H
#define D_A_NPC_SORAJIMA_MALE3_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSma3_c : public dAcNpc_c { 
public:
	dAcNpcSma3_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSma3_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSma3_c);
};

#endif
