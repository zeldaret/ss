#ifndef D_A_OBJ_CHANDELIER_H
#define D_A_OBJ_CHANDELIER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOChandelier_c : public dAcObjBase_c { 
public:
	dAcOChandelier_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOChandelier_c() {}

	STATE_FUNC_DECLARE(dAcOChandelier_c, Wait);
	STATE_FUNC_DECLARE(dAcOChandelier_c, Fall);
	STATE_FUNC_DECLARE(dAcOChandelier_c, End);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOChandelier_c);
};
#endif