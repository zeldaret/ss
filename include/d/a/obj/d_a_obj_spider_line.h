#ifndef D_A_OBJ_SPIDER_LINE_H
#define D_A_OBJ_SPIDER_LINE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dObjSpiderLine_c : public dAcObjBase_c { 
public:
	dObjSpiderLine_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dObjSpiderLine_c() {}

	STATE_FUNC_DECLARE(dObjSpiderLine_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dObjSpiderLine_c);
};
#endif