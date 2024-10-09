#ifndef D_A_OBJ_BALLISTA_F3_H
#define D_A_OBJ_BALLISTA_F3_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBallistaF3_c : public dAcObjBase_c { 
public:
	dAcOBallistaF3_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOBallistaF3_c() {}

	STATE_FUNC_DECLARE(dAcOBallistaF3_c, Wait);
	STATE_FUNC_DECLARE(dAcOBallistaF3_c, Shot);
	STATE_FUNC_DECLARE(dAcOBallistaF3_c, Shot2);
	STATE_FUNC_DECLARE(dAcOBallistaF3_c, ShotEnd);
	STATE_FUNC_DECLARE(dAcOBallistaF3_c, FallStone);
	STATE_FUNC_DECLARE(dAcOBallistaF3_c, EndDemo);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOBallistaF3_c);
};

#endif
