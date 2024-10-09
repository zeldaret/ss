#ifndef D_A_OBJ_TRUCK_RAIL_COL_H
#define D_A_OBJ_TRUCK_RAIL_COL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTruckRailCol_c : public dAcObjBase_c { 
public:
	dAcOTruckRailCol_c();
	virtual ~dAcOTruckRailCol_c();

	STATE_FUNC_DECLARE(dAcOTruckRailCol_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTruckRailCol_c);
};

#endif
