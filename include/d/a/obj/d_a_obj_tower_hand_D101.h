#ifndef D_A_OBJ_TOWER_HAND_D101_H
#define D_A_OBJ_TOWER_HAND_D101_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTowerHandD101_c : public dAcObjBase_c { 
public:
	dAcOTowerHandD101_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTowerHandD101_c() {}

	STATE_FUNC_DECLARE(dAcOTowerHandD101_c, RemainOpen);
	STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Close);
	STATE_FUNC_DECLARE(dAcOTowerHandD101_c, RemainClosed);
	STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Open);
	STATE_FUNC_DECLARE(dAcOTowerHandD101_c, Hold);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTowerHandD101_c);
};
#endif