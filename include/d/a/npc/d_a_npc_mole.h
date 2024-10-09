#ifndef D_A_NPC_MOLE_H
#define D_A_NPC_MOLE_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcMole_c : public dAcNpc_c { 
public:
	dAcNpcMole_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcMole_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcMole_c);
};

#endif
