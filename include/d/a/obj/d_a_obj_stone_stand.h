#ifndef D_A_OBJ_STONE_STAND_H
#define D_A_OBJ_STONE_STAND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOStoneStand_c : public dAcObjBase_c { 
public:
	dAcOStoneStand_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOStoneStand_c() {}

	STATE_FUNC_DECLARE(dAcOStoneStand_c, Wait);
	STATE_FUNC_DECLARE(dAcOStoneStand_c, Shake);
	STATE_FUNC_DECLARE(dAcOStoneStand_c, OnSwitch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOStoneStand_c);
};

#endif
