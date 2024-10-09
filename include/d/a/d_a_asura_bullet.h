#ifndef D_A_ASURA_BULLET_H
#define D_A_ASURA_BULLET_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcAsuraBullet_c : public dAcObjBase_c { 
public:
	dAcAsuraBullet_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcAsuraBullet_c() {}

	STATE_FUNC_DECLARE(dAcAsuraBullet_c, Attack);
	STATE_FUNC_DECLARE(dAcAsuraBullet_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcAsuraBullet_c);
};
#endif