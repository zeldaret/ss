#ifndef D_A_OBJ_TERRY_BICYCLE_H
#define D_A_OBJ_TERRY_BICYCLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTerryBicycle_c : public dAcObjBase_c { 
public:
	dAcOTerryBicycle_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTerryBicycle_c() {}

	STATE_FUNC_DECLARE(dAcOTerryBicycle_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTerryBicycle_c);
};

#endif
