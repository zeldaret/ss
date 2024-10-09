#ifndef D_A_OBJ_IVY_ROPE_H
#define D_A_OBJ_IVY_ROPE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOivyRope_c : public dAcObjBase_c { 
public:
	dAcOivyRope_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOivyRope_c() {}

	STATE_FUNC_DECLARE(dAcOivyRope_c, RopeWait);
	STATE_FUNC_DECLARE(dAcOivyRope_c, PlayerGrip);
	STATE_FUNC_DECLARE(dAcOivyRope_c, RopeReturn);
	STATE_FUNC_DECLARE(dAcOivyRope_c, RopeCut);
	STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeWait);
	STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeUp);
	STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeDown);
	STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopeInit);
	STATE_FUNC_DECLARE(dAcOivyRope_c, TerryRopePlayerCarry);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOivyRope_c);
};

#endif
