#ifndef D_A_OBJ_TROLLEY_SHUTTER_H
#define D_A_OBJ_TROLLEY_SHUTTER_H

#include <d/a/obj/d_a_obj_door_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTrolleyShutter_c : public dAcObjDoor_c { 
public:
	dAcOTrolleyShutter_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTrolleyShutter_c() {}

	STATE_FUNC_DECLARE(dAcOTrolleyShutter_c, Wait);
	STATE_FUNC_DECLARE(dAcOTrolleyShutter_c, OpenEvent);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTrolleyShutter_c);
};
#endif