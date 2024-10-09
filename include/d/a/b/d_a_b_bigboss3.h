#ifndef D_A_B_BIGBOSS3_H
#define D_A_B_BIGBOSS3_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBBigBoss3_c : public dAcEnBase_c { 
public:
	dAcBBigBoss3_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcBBigBoss3_c() {}

	STATE_FUNC_DECLARE(dAcBBigBoss3_c, AppearWait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, AppearDemo);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Wait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Walk);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Reach);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Turn);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, ImpaAdvice);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, ShakeHead);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, HeadDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, HeadDamageThrow);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, HeadPain);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StunPain);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, DropByHand);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Attack);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FootBroken);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, BombHit);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FootRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, HeadRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, ArmBroken);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, ArmRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Stun);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StunWait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StunRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StunDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StunBombDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, LevelUp);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, GuardBallista);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, ArmAttack);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, PreFly);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, PreFlyDemo);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Fly);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyPain);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyFall);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyFallGround);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyFallGroundWait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyFallRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, FlyReach);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, Evacuation);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, StoneWait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, SwordGameWait);
	STATE_FUNC_DECLARE(dAcBBigBoss3_c, SwordGameEnd);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcBBigBoss3_c);
};
#endif