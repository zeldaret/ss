#ifndef D_A_OBJ_GIRAHIMU_KNIFE_H
#define D_A_OBJ_GIRAHIMU_KNIFE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObjGirahimuKnife_c : public dAcEnBase_c { 
public:
	dAcObjGirahimuKnife_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcObjGirahimuKnife_c() {}

	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Wait);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinWait);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinWaitPreAttack);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, SpinFreeWait);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, FreeWait);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Attack);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, AttackEnd);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Return);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, Hit);
	STATE_FUNC_DECLARE(dAcObjGirahimuKnife_c, CircleWait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcObjGirahimuKnife_c);
};

#endif
