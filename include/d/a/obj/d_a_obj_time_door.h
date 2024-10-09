#ifndef D_A_OBJ_TIME_DOOR_H
#define D_A_OBJ_TIME_DOOR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTimeDoor_c : public dAcObjBase_c { 
public:
	dAcOTimeDoor_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTimeDoor_c() {}

	STATE_FUNC_DECLARE(dAcOTimeDoor_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTimeDoor_c);
};
#endif