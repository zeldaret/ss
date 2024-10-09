#ifndef D_A_OBJ_FARMLAND_H
#define D_A_OBJ_FARMLAND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFarmLand_c : public dAcObjBase_c { 
public:
	dAcOFarmLand_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOFarmLand_c() {}

	STATE_FUNC_DECLARE(dAcOFarmLand_c, Wait);
	STATE_FUNC_DECLARE(dAcOFarmLand_c, DummyEvent);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOFarmLand_c);
};
#endif