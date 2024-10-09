#ifndef D_A_OBJ_ROCK_DRAGON_H
#define D_A_OBJ_ROCK_DRAGON_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcORockDragon_c : public dAcObjBase_c { 
public:
	dAcORockDragon_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcORockDragon_c() {}

	STATE_FUNC_DECLARE(dAcORockDragon_c, Upper_Wait);
	STATE_FUNC_DECLARE(dAcORockDragon_c, Upper_Move);
	STATE_FUNC_DECLARE(dAcORockDragon_c, Upper_End);
	STATE_FUNC_DECLARE(dAcORockDragon_c, Lower_Wait);
	STATE_FUNC_DECLARE(dAcORockDragon_c, Lower_RockCatch);
	STATE_FUNC_DECLARE(dAcORockDragon_c, Lower_End);
	STATE_FUNC_DECLARE(dAcORockDragon_c, BossRoom_Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcORockDragon_c);
};
#endif