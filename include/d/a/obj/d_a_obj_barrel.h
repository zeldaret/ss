#ifndef D_A_OBJ_BARREL_H
#define D_A_OBJ_BARREL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBarrel_c : public dAcObjBase_c { 
public:
	dAcOBarrel_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOBarrel_c() {}

	STATE_FUNC_DECLARE(dAcOBarrel_c, Wait);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Grab);
	STATE_FUNC_DECLARE(dAcOBarrel_c, GrabUp);
	STATE_FUNC_DECLARE(dAcOBarrel_c, GrabPut);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Slope);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Water);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Water2);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Explode);
	STATE_FUNC_DECLARE(dAcOBarrel_c, KrakenBorn);
	STATE_FUNC_DECLARE(dAcOBarrel_c, Rebirth);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOBarrel_c);
};
#endif