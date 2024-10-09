#ifndef D_A_LITTLE_BIRD_H
#define D_A_LITTLE_BIRD_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcLittleBird_c : public dAcObjBase_c { 
public:
	dAcLittleBird_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcLittleBird_c() {}

	STATE_FUNC_DECLARE(dAcLittleBird_c, Init);
	STATE_FUNC_DECLARE(dAcLittleBird_c, Wait);
	STATE_FUNC_DECLARE(dAcLittleBird_c, Scratch);
	STATE_FUNC_DECLARE(dAcLittleBird_c, Hop);
	STATE_FUNC_DECLARE(dAcLittleBird_c, Eat);
	STATE_FUNC_DECLARE(dAcLittleBird_c, Sing);
	STATE_FUNC_DECLARE(dAcLittleBird_c, PreEscape);
	STATE_FUNC_DECLARE(dAcLittleBird_c, PreEscape2);
	STATE_FUNC_DECLARE(dAcLittleBird_c, FlyReady);
	STATE_FUNC_DECLARE(dAcLittleBird_c, FlyEscape);
	STATE_FUNC_DECLARE(dAcLittleBird_c, FlyKakku);
	STATE_FUNC_DECLARE(dAcLittleBird_c, FlyReturn);
	STATE_FUNC_DECLARE(dAcLittleBird_c, LandOn);
	STATE_FUNC_DECLARE(dAcLittleBird_c, LandReady);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcLittleBird_c);
};

#endif
