#ifndef D_A_NPC_KOBUN2_H
#define D_A_NPC_KOBUN2_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKbn2_c : public dAcNpc_c { 
public:
	dAcNpcKbn2_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNpcKbn2_c() {}

	STATE_FUNC_DECLARE(dAcNpcKbn2_c, RaceInit);
	STATE_FUNC_DECLARE(dAcNpcKbn2_c, RaceRun);
	STATE_FUNC_DECLARE(dAcNpcKbn2_c, RaceDive);
	STATE_FUNC_DECLARE(dAcNpcKbn2_c, RaceRide);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNpcKbn2_c);
};
#endif