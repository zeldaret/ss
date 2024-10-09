#ifndef D_A_NPC_SHINKAN2_H
#define D_A_NPC_SHINKAN2_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSkn2_c : public dAcNpc_c { 
public:
	dAcNpcSkn2_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSkn2_c() {}

	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch0);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, WatchLerp0);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch1);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch2);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch3);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, TurnRemly);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, Watch4);
	STATE_FUNC_DECLARE(dAcNpcSkn2_c, WatchLerp);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSkn2_c);
};

#endif
