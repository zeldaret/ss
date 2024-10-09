#ifndef D_A_NPC_MOLE_SAL_H
#define D_A_NPC_MOLE_SAL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcMoleSal_c : public dAcNpc_c { 
public:
	dAcNpcMoleSal_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcMoleSal_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleSal_c);
};

#endif
