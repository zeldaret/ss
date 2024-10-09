#ifndef D_A_NPC_MOLE_NORMAL_H
#define D_A_NPC_MOLE_NORMAL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class NPC_MOLE_NML_CLASS : public dAcNpc_c { 
public:
	NPC_MOLE_NML_CLASS() : mStateMgr(*this, sStateID::null) {}
	virtual ~NPC_MOLE_NML_CLASS() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(NPC_MOLE_NML_CLASS);
};
#endif