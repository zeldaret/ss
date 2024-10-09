#ifndef D_A_OBJ_SHUTTER_WATERD101_H
#define D_A_OBJ_SHUTTER_WATERD101_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOShutterWaterD101_c : public dAcObjBase_c { 
public:
	dAcOShutterWaterD101_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOShutterWaterD101_c() {}

	STATE_FUNC_DECLARE(dAcOShutterWaterD101_c, Wait);
	STATE_FUNC_DECLARE(dAcOShutterWaterD101_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOShutterWaterD101_c);
};
#endif