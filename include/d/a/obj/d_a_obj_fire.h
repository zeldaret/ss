#ifndef D_A_OBJ_FIRE_H
#define D_A_OBJ_FIRE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOfire_c : public dAcObjBase_c { 
public:
	dAcOfire_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOfire_c() {}

	STATE_FUNC_DECLARE(dAcOfire_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOfire_c);
};

#endif
