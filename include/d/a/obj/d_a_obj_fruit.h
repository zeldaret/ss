#ifndef D_A_OBJ_FRUIT_H
#define D_A_OBJ_FRUIT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFruit_c : public dAcObjBase_c { 
public:
	dAcOFruit_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOFruit_c() {}

	STATE_FUNC_DECLARE(dAcOFruit_c, NormalWait);
	STATE_FUNC_DECLARE(dAcOFruit_c, StickWait);
	STATE_FUNC_DECLARE(dAcOFruit_c, GrabCarry);
	STATE_FUNC_DECLARE(dAcOFruit_c, SwordCarry);
	STATE_FUNC_DECLARE(dAcOFruit_c, BoomerangCarry);
	STATE_FUNC_DECLARE(dAcOFruit_c, WhipCarry);
	STATE_FUNC_DECLARE(dAcOFruit_c, Dead);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOFruit_c);
};

#endif
