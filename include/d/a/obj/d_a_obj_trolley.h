#ifndef D_A_OBJ_TROLLEY_H
#define D_A_OBJ_TROLLEY_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTrolley_c : public dAcObjBase_c { 
public:
	dAcOTrolley_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTrolley_c() {}

	STATE_FUNC_DECLARE(dAcOTrolley_c, Wait);
	STATE_FUNC_DECLARE(dAcOTrolley_c, RideAction);
	STATE_FUNC_DECLARE(dAcOTrolley_c, StartAction);
	STATE_FUNC_DECLARE(dAcOTrolley_c, PathMove);
	STATE_FUNC_DECLARE(dAcOTrolley_c, StopAction);
	STATE_FUNC_DECLARE(dAcOTrolley_c, CrashAction);
	STATE_FUNC_DECLARE(dAcOTrolley_c, PushPull);
	STATE_FUNC_DECLARE(dAcOTrolley_c, Fall);
	STATE_FUNC_DECLARE(dAcOTrolley_c, End);
	STATE_FUNC_DECLARE(dAcOTrolley_c, TimeSlipStart);
	STATE_FUNC_DECLARE(dAcOTrolley_c, TimeSlipEnd);
	STATE_FUNC_DECLARE(dAcOTrolley_c, WaitOpenDoor);
	STATE_FUNC_DECLARE(dAcOTrolley_c, RestartWait);
	STATE_FUNC_DECLARE(dAcOTrolley_c, BuriedSand);
	STATE_FUNC_DECLARE(dAcOTrolley_c, BackMove);
	STATE_FUNC_DECLARE(dAcOTrolley_c, F302RevivalDemo);
	STATE_FUNC_DECLARE(dAcOTrolley_c, TimeSlipEndFromAnother);
	STATE_FUNC_DECLARE(dAcOTrolley_c, WaitOnSwitch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTrolley_c);
};

#endif
