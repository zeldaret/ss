#ifndef D_A_B_NUSI_BULLET_H
#define D_A_B_NUSI_BULLET_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNusi_Bullet_c : public dAcEnBase_c { 
public:
	dAcNusi_Bullet_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcNusi_Bullet_c() {}

	STATE_FUNC_DECLARE(dAcNusi_Bullet_c, Move);
	STATE_FUNC_DECLARE(dAcNusi_Bullet_c, Wait);
	STATE_FUNC_DECLARE(dAcNusi_Bullet_c, Back);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcNusi_Bullet_c);
};

#endif
