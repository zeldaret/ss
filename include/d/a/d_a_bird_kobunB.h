#ifndef D_A_BIRD_KOBUNB_H
#define D_A_BIRD_KOBUNB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dBird_KobunB_c : public dAcObjBase_c { 
public:
	dBird_KobunB_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dBird_KobunB_c() {}

	STATE_FUNC_DECLARE(dBird_KobunB_c, Wait);
	STATE_FUNC_DECLARE(dBird_KobunB_c, Chase);
	STATE_FUNC_DECLARE(dBird_KobunB_c, FrontAttack);
	STATE_FUNC_DECLARE(dBird_KobunB_c, PathMove);
	STATE_FUNC_DECLARE(dBird_KobunB_c, BodyAttack);
	STATE_FUNC_DECLARE(dBird_KobunB_c, LevelUpDemo);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dBird_KobunB_c);
};

#endif
