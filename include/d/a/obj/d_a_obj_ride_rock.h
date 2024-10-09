#ifndef D_A_OBJ_RIDE_ROCK_H
#define D_A_OBJ_RIDE_ROCK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOrideRock_c : public dAcObjBase_c { 
public:
	dAcOrideRock_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOrideRock_c() {}

	STATE_FUNC_DECLARE(dAcOrideRock_c, Wait);
	STATE_FUNC_DECLARE(dAcOrideRock_c, Slope);
	STATE_FUNC_DECLARE(dAcOrideRock_c, SlopeWait);
	STATE_FUNC_DECLARE(dAcOrideRock_c, RideMove);
	STATE_FUNC_DECLARE(dAcOrideRock_c, SwitchMove);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOrideRock_c);
};

#endif
