#ifndef D_A_NPC_KENSEI_H
#define D_A_NPC_KENSEI_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKensei_c : public dAcNpc_c { 
public:
	dAcNpcKensei_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcKensei_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcKensei_c);
};

#endif
