#ifndef D_A_NPC_SUISEI_SUB_H
#define D_A_NPC_SUISEI_SUB_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSuiseiSub_c : public dAcNpc_c { 
public:
	dAcNpcSuiseiSub_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSuiseiSub_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSuiseiSub_c);
};

#endif
