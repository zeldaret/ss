#ifndef D_A_OBJ_SWRD_PRJ_H
#define D_A_OBJ_SWRD_PRJ_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwrdPrj : public dAcObjBase_c { 
public:
	dAcOSwrdPrj();
	virtual ~dAcOSwrdPrj();

	STATE_FUNC_DECLARE(dAcOSwrdPrj, WaitHarp);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, WaitAppear);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, Wait);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, WaitCross);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, ChkTri);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, End);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, SWWait);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, SWWaitCross);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, SWChkTri);
	STATE_FUNC_DECLARE(dAcOSwrdPrj, SWEnd);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOSwrdPrj);
};
#endif