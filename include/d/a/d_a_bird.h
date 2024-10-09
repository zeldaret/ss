#ifndef D_A_BIRD_H
#define D_A_BIRD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dBird_c : public dAcObjBase_c { 
public:
	dBird_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dBird_c() {}

	STATE_FUNC_DECLARE(dBird_c, Wait);
	STATE_FUNC_DECLARE(dBird_c, PreMove);
	STATE_FUNC_DECLARE(dBird_c, Move);
	STATE_FUNC_DECLARE(dBird_c, Hide);
	STATE_FUNC_DECLARE(dBird_c, RideDemo);
	STATE_FUNC_DECLARE(dBird_c, FirstRideDemo);
	STATE_FUNC_DECLARE(dBird_c, OutAreaDemo);
	STATE_FUNC_DECLARE(dBird_c, RideOff);
	STATE_FUNC_DECLARE(dBird_c, OnGround);
	STATE_FUNC_DECLARE(dBird_c, Tornado);
	STATE_FUNC_DECLARE(dBird_c, SaveStartDemo);
	STATE_FUNC_DECLARE(dBird_c, MiniGameWait);
	STATE_FUNC_DECLARE(dBird_c, MiniGameMove);
	STATE_FUNC_DECLARE(dBird_c, MiniGameEnd);
	STATE_FUNC_DECLARE(dBird_c, MiniGameLevelUpDemo);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dBird_c);
};
#endif