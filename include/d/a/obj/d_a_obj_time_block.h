#ifndef D_A_OBJ_TIME_BLOCK_H
#define D_A_OBJ_TIME_BLOCK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTimeBlock_c : public dAcObjBase_c { 
public:
	dAcOTimeBlock_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTimeBlock_c() {}

	STATE_FUNC_DECLARE(dAcOTimeBlock_c, Wait);
	STATE_FUNC_DECLARE(dAcOTimeBlock_c, TimeSlip);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTimeBlock_c);
};
#endif