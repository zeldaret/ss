#ifndef D_A_NPC_THUNDER_DRAGON_SKULL_H
#define D_A_NPC_THUNDER_DRAGON_SKULL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcTds_c : public dAcNpc_c { 
public:
	dAcNpcTds_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcTds_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcTds_c);
};
#endif