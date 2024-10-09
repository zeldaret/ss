#ifndef D_A_NPC_TERRY_H
#define D_A_NPC_TERRY_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcTerry_c : public dAcNpc_c { 
public:
	dAcNpcTerry_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcTerry_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcTerry_c);
};
#endif