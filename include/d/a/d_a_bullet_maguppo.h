#ifndef D_A_BULLET_MAGUPPO_H
#define D_A_BULLET_MAGUPPO_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBulletMaguppo_c : public dAcObjBase_c { 
public:
	dAcBulletMaguppo_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcBulletMaguppo_c() {}

	STATE_FUNC_DECLARE(dAcBulletMaguppo_c, Move);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcBulletMaguppo_c);
};
#endif