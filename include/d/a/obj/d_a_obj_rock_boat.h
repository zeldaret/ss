#ifndef D_A_OBJ_ROCK_BOAT_H
#define D_A_OBJ_ROCK_BOAT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcORockBoat_c : public dAcObjBase_c { 
public:
	dAcORockBoat_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcORockBoat_c() {}

	STATE_FUNC_DECLARE(dAcORockBoat_c, Appear);
	STATE_FUNC_DECLARE(dAcORockBoat_c, Wait);
	STATE_FUNC_DECLARE(dAcORockBoat_c, Move);
	STATE_FUNC_DECLARE(dAcORockBoat_c, PreSink);
	STATE_FUNC_DECLARE(dAcORockBoat_c, Sink);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcORockBoat_c);
};
#endif