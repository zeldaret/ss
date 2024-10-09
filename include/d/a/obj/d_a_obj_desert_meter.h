#ifndef D_A_OBJ_DESERT_METER_H
#define D_A_OBJ_DESERT_METER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODesertMeter_c : public dAcObjBase_c { 
public:
	dAcODesertMeter_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODesertMeter_c() {}

	STATE_FUNC_DECLARE(dAcODesertMeter_c, Wait);
	STATE_FUNC_DECLARE(dAcODesertMeter_c, OnSwitch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODesertMeter_c);
};

#endif
