#ifndef D_A_NPC_KYUI_ELDER_H
#define D_A_NPC_KYUI_ELDER_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKyuiElder_c : public dAcNpc_c { 
public:
	dAcNpcKyuiElder_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcKyuiElder_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcKyuiElder_c);
};
#endif