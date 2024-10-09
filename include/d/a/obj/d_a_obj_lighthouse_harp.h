#ifndef D_A_OBJ_LIGHTHOUSE_HARP_H
#define D_A_OBJ_LIGHTHOUSE_HARP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOlighthouseHarp_c : public dAcObjBase_c { 
public:
	dAcOlighthouseHarp_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcOlighthouseHarp_c() {}

	STATE_FUNC_DECLARE(dAcOlighthouseHarp_c, Wait);
	STATE_FUNC_DECLARE(dAcOlighthouseHarp_c, Move);
	STATE_FUNC_DECLARE(dAcOlighthouseHarp_c, Appear);
	STATE_FUNC_DECLARE(dAcOlighthouseHarp_c, Roll);
	STATE_FUNC_DECLARE(dAcOlighthouseHarp_c, HarpFinish);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcOlighthouseHarp_c);
};

#endif
