#ifndef D_A_OBJ_TERRY_HOLE_H
#define D_A_OBJ_TERRY_HOLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTerryHole_c : public dAcObjBase_c { 
public:
	dAcOTerryHole_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTerryHole_c() {}

	STATE_FUNC_DECLARE(dAcOTerryHole_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTerryHole_c);
};
#endif