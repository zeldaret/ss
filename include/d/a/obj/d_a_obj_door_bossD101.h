#ifndef D_A_OBJ_DOOR_BOSSD101_H
#define D_A_OBJ_DOOR_BOSSD101_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcODoorBossD101_c : public dAcObjBase_c { 
public:
	dAcODoorBossD101_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcODoorBossD101_c() {}

	STATE_FUNC_DECLARE(dAcODoorBossD101_c, Wait);
	STATE_FUNC_DECLARE(dAcODoorBossD101_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcODoorBossD101_c);
};
#endif