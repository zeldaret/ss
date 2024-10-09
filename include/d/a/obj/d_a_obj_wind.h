#ifndef D_A_OBJ_WIND_H
#define D_A_OBJ_WIND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwind_c : public dAcObjBase_c { 
public:
	dAcOwind_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwind_c() {}

	STATE_FUNC_DECLARE(dAcOwind_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwind_c);
};

#endif
