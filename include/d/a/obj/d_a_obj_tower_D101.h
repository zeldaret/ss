#ifndef D_A_OBJ_TOWER_D101_H
#define D_A_OBJ_TOWER_D101_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTowerD101_c : public dAcObjBase_c { 
public:
	dAcOTowerD101_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTowerD101_c() {}

	STATE_FUNC_DECLARE(dAcOTowerD101_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTowerD101_c);
};
#endif