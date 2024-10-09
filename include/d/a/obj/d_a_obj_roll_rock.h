#ifndef D_A_OBJ_ROLL_ROCK_H
#define D_A_OBJ_ROLL_ROCK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOrollRock_c : public dAcObjBase_c { 
public:
	dAcOrollRock_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOrollRock_c() {}

	STATE_FUNC_DECLARE(dAcOrollRock_c, Wait);
	STATE_FUNC_DECLARE(dAcOrollRock_c, Slope);
	STATE_FUNC_DECLARE(dAcOrollRock_c, SlopeWait);
	STATE_FUNC_DECLARE(dAcOrollRock_c, SwitchStop);
	STATE_FUNC_DECLARE(dAcOrollRock_c, PathMove);
	STATE_FUNC_DECLARE(dAcOrollRock_c, PathJump);
	STATE_FUNC_DECLARE(dAcOrollRock_c, PathJumpArrive);
	STATE_FUNC_DECLARE(dAcOrollRock_c, GrabCarry);
	STATE_FUNC_DECLARE(dAcOrollRock_c, StopSink);
	STATE_FUNC_DECLARE(dAcOrollRock_c, BossDemoWait);
	STATE_FUNC_DECLARE(dAcOrollRock_c, BossDemoPathMove);
	STATE_FUNC_DECLARE(dAcOrollRock_c, BossDemoStop);
	STATE_FUNC_DECLARE(dAcOrollRock_c, BossDemoPreWait);
	STATE_FUNC_DECLARE(dAcOrollRock_c, BossDemoPrePathMove);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOrollRock_c);
};

#endif
