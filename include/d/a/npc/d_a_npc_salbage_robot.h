#ifndef D_A_NPC_SALBAGE_ROBOT_H
#define D_A_NPC_SALBAGE_ROBOT_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSlb_c : public dAcNpc_c { 
public:
	dAcNpcSlb_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSlb_c() {}

	STATE_FUNC_DECLARE(dAcNpcSlb_c, Hide);
	STATE_FUNC_DECLARE(dAcNpcSlb_c, HideDelivary);
	STATE_FUNC_DECLARE(dAcNpcSlb_c, DelivaryReq);
	STATE_FUNC_DECLARE(dAcNpcSlb_c, Kill);
	STATE_FUNC_DECLARE(dAcNpcSlb_c, Kill2);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSlb_c);
};

#endif
