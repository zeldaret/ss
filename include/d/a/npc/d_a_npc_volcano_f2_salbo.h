#ifndef D_A_NPC_VOLCANO_F2_SALBO_H
#define D_A_NPC_VOLCANO_F2_SALBO_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSlb2_c : public dAcNpc_c { 
public:
	dAcNpcSlb2_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSlb2_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSlb2_c);
};

#endif
