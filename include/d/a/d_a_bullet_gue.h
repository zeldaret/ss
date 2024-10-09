#ifndef D_A_BULLET_GUE_H
#define D_A_BULLET_GUE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBulletGue_c : public dAcObjBase_c { 
public:
	dAcBulletGue_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcBulletGue_c() {}

	STATE_FUNC_DECLARE(dAcBulletGue_c, Wait);
	STATE_FUNC_DECLARE(dAcBulletGue_c, Hold);
	STATE_FUNC_DECLARE(dAcBulletGue_c, TakeOff);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcBulletGue_c);
};

#endif
