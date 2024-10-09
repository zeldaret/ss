#ifndef D_A_OBJ_TOWER_BOMB_H
#define D_A_OBJ_TOWER_BOMB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTowerBomb_c : public dAcObjBase_c { 
public:
	dAcOTowerBomb_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTowerBomb_c() {}

	STATE_FUNC_DECLARE(dAcOTowerBomb_c, Wait);
	STATE_FUNC_DECLARE(dAcOTowerBomb_c, Bomb);
	STATE_FUNC_DECLARE(dAcOTowerBomb_c, Broken);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTowerBomb_c);
};
#endif