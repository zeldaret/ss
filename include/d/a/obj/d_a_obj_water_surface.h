#ifndef D_A_OBJ_WATER_SURFACE_H
#define D_A_OBJ_WATER_SURFACE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwaterSurface_c : public dAcObjBase_c { 
public:
	dAcOwaterSurface_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOwaterSurface_c() {}

	STATE_FUNC_DECLARE(dAcOwaterSurface_c, Wait);
	STATE_FUNC_DECLARE(dAcOwaterSurface_c, Move);
	STATE_FUNC_DECLARE(dAcOwaterSurface_c, After1);
	STATE_FUNC_DECLARE(dAcOwaterSurface_c, After2);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOwaterSurface_c);
};

#endif
