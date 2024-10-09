#ifndef D_A_OBJ_TREASURE_ISLAND_B_H
#define D_A_OBJ_TREASURE_ISLAND_B_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOtreasureIslandB_c : public dAcObjBase_c { 
public:
	dAcOtreasureIslandB_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOtreasureIslandB_c() {}

	STATE_FUNC_DECLARE(dAcOtreasureIslandB_c, Wait);
	STATE_FUNC_DECLARE(dAcOtreasureIslandB_c, CoverBreak);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOtreasureIslandB_c);
};

#endif
