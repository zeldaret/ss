#ifndef D_A_E_RUPEE_GUE_H
#define D_A_E_RUPEE_GUE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcErupeeGue_c : public dAcEnBase_c { 
public:
	dAcErupeeGue_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcErupeeGue_c() {}

	STATE_FUNC_DECLARE(dAcErupeeGue_c, Wait);
	STATE_FUNC_DECLARE(dAcErupeeGue_c, RunAway);
	STATE_FUNC_DECLARE(dAcErupeeGue_c, Hit);
	STATE_FUNC_DECLARE(dAcErupeeGue_c, GHit);
	STATE_FUNC_DECLARE(dAcErupeeGue_c, Hide);
	STATE_FUNC_DECLARE(dAcErupeeGue_c, Death);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcErupeeGue_c);
};
#endif