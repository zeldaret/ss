#ifndef D_A_OBJ_TABLEWARE_H
#define D_A_OBJ_TABLEWARE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOTableWare_c : public dAcObjBase_c { 
public:
	dAcOTableWare_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOTableWare_c() {}

	STATE_FUNC_DECLARE(dAcOTableWare_c, Wait);
	STATE_FUNC_DECLARE(dAcOTableWare_c, Down);
	STATE_FUNC_DECLARE(dAcOTableWare_c, Drop);
	STATE_FUNC_DECLARE(dAcOTableWare_c, WindMove);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOTableWare_c);
};
#endif