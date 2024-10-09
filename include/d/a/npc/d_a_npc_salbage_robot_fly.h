#ifndef D_A_NPC_SALBAGE_ROBOT_FLY_H
#define D_A_NPC_SALBAGE_ROBOT_FLY_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSalbageRobotFly_c : public dAcNpc_c { 
public:
	dAcNpcSalbageRobotFly_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcSalbageRobotFly_c() {}

	STATE_FUNC_DECLARE(dAcNpcSalbageRobotFly_c, ChasePlayerBird);
	STATE_FUNC_DECLARE(dAcNpcSalbageRobotFly_c, FlyWithPlayerBird);
	STATE_FUNC_DECLARE(dAcNpcSalbageRobotFly_c, Leave);
	STATE_FUNC_DECLARE(dAcNpcSalbageRobotFly_c, EscapeToUpward);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcSalbageRobotFly_c);
};
#endif