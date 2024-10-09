#ifndef D_A_OBJ_DOOR_BOSS_H
#define D_A_OBJ_DOOR_BOSS_H

#include <d/a/obj/d_a_obj_door_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOdoorBoss_c : public dAcObjDoor_c { 
public:
	dAcOdoorBoss_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOdoorBoss_c() {}

	STATE_FUNC_DECLARE(dAcOdoorBoss_c, UnlockWait);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, CorrectKey);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, KeyInsertInitialInterval);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, PutKeyIntoLock);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, KeyInsert);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, KeyInsertAfterglow);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Wait);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Shine);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Unlock);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, OpenWait);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Open);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, WaitClose);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Close);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, Idle);
	STATE_FUNC_DECLARE(dAcOdoorBoss_c, BeforeTimeShift);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOdoorBoss_c);
};
#endif