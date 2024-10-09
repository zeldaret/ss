#ifndef D_A_OBJ_SHUTTER_H
#define D_A_OBJ_SHUTTER_H

#include <d/a/obj/d_a_obj_door_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOShutter_c : public dAcObjDoor_c { 
public:
	dAcOShutter_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOShutter_c() {}

	STATE_FUNC_DECLARE(dAcOShutter_c, Wait);
	STATE_FUNC_DECLARE(dAcOShutter_c, Open);
	STATE_FUNC_DECLARE(dAcOShutter_c, Close);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOShutter_c);
};

#endif
