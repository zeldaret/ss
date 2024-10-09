#ifndef D_A_OBJ_PISTON_H
#define D_A_OBJ_PISTON_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOPiston_c : public dAcObjBase_c { 
public:
	dAcOPiston_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOPiston_c() {}

	STATE_FUNC_DECLARE(dAcOPiston_c, WaitDelay);
	STATE_FUNC_DECLARE(dAcOPiston_c, OnAnim);
	STATE_FUNC_DECLARE(dAcOPiston_c, Wait);
	STATE_FUNC_DECLARE(dAcOPiston_c, Move);
	STATE_FUNC_DECLARE(dAcOPiston_c, Event);
	STATE_FUNC_DECLARE(dAcOPiston_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOPiston_c);
};

#endif
