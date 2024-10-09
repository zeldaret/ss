#ifndef D_A_OBJ_TERRY_GIMMICK_H
#define D_A_OBJ_TERRY_GIMMICK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTerryGimmick_c : public dAcObjBase_c { 
public:
	dAcOTerryGimmick_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTerryGimmick_c() {}

	STATE_FUNC_DECLARE(dAcOTerryGimmick_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTerryGimmick_c);
};
#endif