#ifndef D_A_OBJ_TOWER_GEARD101_H
#define D_A_OBJ_TOWER_GEARD101_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTowerGearD101_c : public dAcObjBase_c { 
public:
	dAcOTowerGearD101_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTowerGearD101_c() {}

	STATE_FUNC_DECLARE(dAcOTowerGearD101_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTowerGearD101_c);
};

#endif
