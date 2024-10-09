#ifndef D_A_OBJ_CLEF_H
#define D_A_OBJ_CLEF_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOClef_c : public dAcObjBase_c { 
public:
	dAcOClef_c();
	virtual ~dAcOClef_c();

	STATE_FUNC_DECLARE(dAcOClef_c, WaitPhantom);
	STATE_FUNC_DECLARE(dAcOClef_c, Wait);
	STATE_FUNC_DECLARE(dAcOClef_c, WaitPathMove);
	STATE_FUNC_DECLARE(dAcOClef_c, MoveTowardPath);
	STATE_FUNC_DECLARE(dAcOClef_c, PathMove);
	STATE_FUNC_DECLARE(dAcOClef_c, Gravitate);
	STATE_FUNC_DECLARE(dAcOClef_c, Die);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOClef_c);
};

#endif
