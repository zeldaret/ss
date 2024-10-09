#ifndef D_A_NPC_REGRET_RIVAL_H
#define D_A_NPC_REGRET_RIVAL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcRegretRvl_c : public dAcNpc_c { 
public:
	dAcNpcRegretRvl_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcRegretRvl_c() {}

	STATE_FUNC_DECLARE(dAcNpcRegretRvl_c, Ready);
	STATE_FUNC_DECLARE(dAcNpcRegretRvl_c, Punch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcRegretRvl_c);
};
#endif