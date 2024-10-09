#ifndef D_T_SKYENEMY_H
#define D_T_SKYENEMY_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgSkyEnemyTag : public dAcBase_c { 
public:
	dTgSkyEnemyTag() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgSkyEnemyTag() {}

	STATE_FUNC_DECLARE(dTgSkyEnemyTag, Wait);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthRupeeGue);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthSkytail);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthTornado);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthBirdNpc);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthFrontBirdNpc);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthNusiNpc);
	STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthHighSpeed);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgSkyEnemyTag);
};

#endif
