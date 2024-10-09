#ifndef D_A_NPC_BBRVL_H
#define D_A_NPC_BBRVL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcBBRvl_c : public dAcNpc_c { 
public:
	dAcNpcBBRvl_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcBBRvl_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcBBRvl_c);
};
#endif