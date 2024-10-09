#ifndef D_A_OBJ_BOAT_H
#define D_A_OBJ_BOAT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBoat_c : public dAcObjBase_c { 
public:
	dAcOBoat_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOBoat_c() {}

	STATE_FUNC_DECLARE(dAcOBoat_c, Wait);
	STATE_FUNC_DECLARE(dAcOBoat_c, Fall);
	STATE_FUNC_DECLARE(dAcOBoat_c, FallEnd);
	STATE_FUNC_DECLARE(dAcOBoat_c, FallStop);
	STATE_FUNC_DECLARE(dAcOBoat_c, Up);
	STATE_FUNC_DECLARE(dAcOBoat_c, UpEnd);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOBoat_c);
};

#endif
