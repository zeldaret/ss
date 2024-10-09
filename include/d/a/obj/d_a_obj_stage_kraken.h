#ifndef D_A_OBJ_STAGE_KRAKEN_H
#define D_A_OBJ_STAGE_KRAKEN_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstageKraken_c : public dAcObjBase_c { 
public:
	dAcOstageKraken_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOstageKraken_c() {}

	STATE_FUNC_DECLARE(dAcOstageKraken_c, Wait);
	STATE_FUNC_DECLARE(dAcOstageKraken_c, MarkLight);
	STATE_FUNC_DECLARE(dAcOstageKraken_c, AroundMarkLight);
	STATE_FUNC_DECLARE(dAcOstageKraken_c, MarkVanish);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOstageKraken_c);
};
#endif