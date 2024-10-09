#ifndef D_A_OBJ_CANNON_H
#define D_A_OBJ_CANNON_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOcannon_c : public dAcObjBase_c { 
public:
	dAcOcannon_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOcannon_c() {}

	STATE_FUNC_DECLARE(dAcOcannon_c, Wait);
	STATE_FUNC_DECLARE(dAcOcannon_c, PreCharge);
	STATE_FUNC_DECLARE(dAcOcannon_c, Move);
	STATE_FUNC_DECLARE(dAcOcannon_c, AfterCharge);
	STATE_FUNC_DECLARE(dAcOcannon_c, Shot);
	STATE_FUNC_DECLARE(dAcOcannon_c, After);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOcannon_c);
};
#endif