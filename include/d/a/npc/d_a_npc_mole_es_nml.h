#ifndef D_A_NPC_MOLE_ES_NML_H
#define D_A_NPC_MOLE_ES_NML_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcMoleEsNml_c : public dAcNpc_c { 
public:
	dAcNpcMoleEsNml_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcMoleEsNml_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleEsNml_c);
};
#endif