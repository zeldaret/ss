#ifndef D_A_NPC_SALBAGE_ROBOT_REPAIR_H
#define D_A_NPC_SALBAGE_ROBOT_REPAIR_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSalbageRobotRepair_c : public dAcNpc_c { 
public:
	dAcNpcSalbageRobotRepair_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSalbageRobotRepair_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSalbageRobotRepair_c);
};

#endif
