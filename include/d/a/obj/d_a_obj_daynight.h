#ifndef D_A_OBJ_DAYNIGHT_H
#define D_A_OBJ_DAYNIGHT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODayNight_c : public dAcObjBase_c { 
public:
	dAcODayNight_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODayNight_c() {}

	STATE_FUNC_DECLARE(dAcODayNight_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODayNight_c);
};
#endif