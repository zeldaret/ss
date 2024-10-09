#ifndef D_A_NPC_YOUNG_IMPA_H
#define D_A_NPC_YOUNG_IMPA_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcYim_c : public dAcNpc_c { 
public:
	dAcNpcYim_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcYim_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcYim_c);
};

#endif
