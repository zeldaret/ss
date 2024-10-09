#ifndef D_A_OBJ_MOVE_LIFT_VOL_H
#define D_A_OBJ_MOVE_LIFT_VOL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOmoveLiftVol_c : public dAcObjBase_c { 
public:
	dAcOmoveLiftVol_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOmoveLiftVol_c() {}

	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, Wait);
	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, Ready);
	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, Move);
	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, Arrive);
	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, SwitchWait);
	STATE_FUNC_DECLARE(dAcOmoveLiftVol_c, RopeMove);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOmoveLiftVol_c);
};

#endif
