#ifndef D_T_SWORD_BATTLE_GAME_H
#define D_T_SWORD_BATTLE_GAME_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgSwordBattleGame_c : public dAcBase_c { 
public:
	dTgSwordBattleGame_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgSwordBattleGame_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgSwordBattleGame_c);
};
#endif