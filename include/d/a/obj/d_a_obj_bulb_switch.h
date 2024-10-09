#ifndef D_A_OBJ_BULB_SWITCH_H
#define D_A_OBJ_BULB_SWITCH_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObulbSwitch_c : public dAcObjBase_c { 
public:
	dAcObulbSwitch_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcObulbSwitch_c() {}

	STATE_FUNC_DECLARE(dAcObulbSwitch_c, Close);
	STATE_FUNC_DECLARE(dAcObulbSwitch_c, Open);
	STATE_FUNC_DECLARE(dAcObulbSwitch_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcObulbSwitch_c);
};

#endif
