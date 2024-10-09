#ifndef D_A_OBJ_PROPELLER_LIFT_H
#define D_A_OBJ_PROPELLER_LIFT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOPropellerLift_c : public dAcObjBase_c { 
public:
	dAcOPropellerLift_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOPropellerLift_c() {}

	STATE_FUNC_DECLARE(dAcOPropellerLift_c, Normal);
	STATE_FUNC_DECLARE(dAcOPropellerLift_c, BackMove);
	STATE_FUNC_DECLARE(dAcOPropellerLift_c, BackMoveMain);
	STATE_FUNC_DECLARE(dAcOPropellerLift_c, CrashAction);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOPropellerLift_c);
};
#endif