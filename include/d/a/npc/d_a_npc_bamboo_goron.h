#ifndef D_A_NPC_BAMBOO_GORON_H
#define D_A_NPC_BAMBOO_GORON_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcBgr_c : public dAcNpc_c { 
public:
	dAcNpcBgr_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcBgr_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcBgr_c);
};

#endif
