#ifndef D_A_AVATER_BULLET_H
#define D_A_AVATER_BULLET_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcAvater_Bullet_c : public dAcObjBase_c { 
public:
	dAcAvater_Bullet_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcAvater_Bullet_c() {}

	STATE_FUNC_DECLARE(dAcAvater_Bullet_c, Move);
	STATE_FUNC_DECLARE(dAcAvater_Bullet_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcAvater_Bullet_c);
};
#endif