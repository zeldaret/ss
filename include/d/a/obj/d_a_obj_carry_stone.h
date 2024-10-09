#ifndef D_A_OBJ_CARRY_STONE_H
#define D_A_OBJ_CARRY_STONE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOcarryStone_c : public dAcObjBase_c { 
public:
	dAcOcarryStone_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOcarryStone_c() {}

	STATE_FUNC_DECLARE(dAcOcarryStone_c, Wait);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, GrabCarry);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, Water);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, Slope);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, SlopeWait);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, MoveVacuum);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, ShakeWind);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, MoveMoleGlove);
	STATE_FUNC_DECLARE(dAcOcarryStone_c, EggEnterSwitch);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOcarryStone_c);
};
#endif