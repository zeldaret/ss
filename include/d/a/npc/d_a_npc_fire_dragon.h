#ifndef D_A_NPC_FIRE_DRAGON_H
#define D_A_NPC_FIRE_DRAGON_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcFdr_c : public dAcNpc_c { 
public:
	dAcNpcFdr_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcFdr_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcFdr_c);
};

#endif
