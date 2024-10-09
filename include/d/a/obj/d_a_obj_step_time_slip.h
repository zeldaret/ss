#ifndef D_A_OBJ_STEP_TIME_SLIP_H
#define D_A_OBJ_STEP_TIME_SLIP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOStepTimeSlip_c : public dAcObjBase_c { 
public:
	dAcOStepTimeSlip_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOStepTimeSlip_c() {}

	STATE_FUNC_DECLARE(dAcOStepTimeSlip_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOStepTimeSlip_c);
};
#endif