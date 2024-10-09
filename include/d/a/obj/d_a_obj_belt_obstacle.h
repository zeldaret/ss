#ifndef D_A_OBJ_BELT_OBSTACLE_H
#define D_A_OBJ_BELT_OBSTACLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObeltObstacle_c : public dAcObjBase_c { 
public:
	dAcObeltObstacle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcObeltObstacle_c() {}

	STATE_FUNC_DECLARE(dAcObeltObstacle_c, Wait);
	STATE_FUNC_DECLARE(dAcObeltObstacle_c, Init);
	STATE_FUNC_DECLARE(dAcObeltObstacle_c, BeltMove);
	STATE_FUNC_DECLARE(dAcObeltObstacle_c, TeniN);
	STATE_FUNC_DECLARE(dAcObeltObstacle_c, TeniNonly);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcObeltObstacle_c);
};

#endif
