#ifndef D_A_B_BIGBOSS2_H
#define D_A_B_BIGBOSS2_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcBBigBoss2_c : public dAcEnBase_c { 
public:
	dAcBBigBoss2_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcBBigBoss2_c() {}

	STATE_FUNC_DECLARE(dAcBBigBoss2_c, AppearWait);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, AppearDemo);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Wait);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Walk);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Reach);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Turn);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, ImpaAdvice);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, ShakeHead);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, HeadDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, HeadDamageThrow);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, HeadPain);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunPain);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, HeadRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, DropByHand);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Attack);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, FootBroken);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, BombHit);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, FootRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, ArmBroken);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, ArmAttack);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, ArmRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Stun);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunWait);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunRecover);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunBombDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Mount);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Fall);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, MountDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, FrontFall);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Rush);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Rise);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Tied);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StunFrontFall);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, BadAdvice);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, CancelMount);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, MountArmBroken);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, LevelUp);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, RushDamage);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, GuardBallista);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, Evacuation);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, StoneWait);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, SwordGameWait);
	STATE_FUNC_DECLARE(dAcBBigBoss2_c, SwordGameEnd);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcBBigBoss2_c);
};

#endif
