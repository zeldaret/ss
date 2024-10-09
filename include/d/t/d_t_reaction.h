#ifndef D_T_REACTION_H
#define D_T_REACTION_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgReaction_c : public dAcObjBase_c { 
public:
	dTgReaction_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgReaction_c() {}

	STATE_FUNC_DECLARE(dTgReaction_c, Wait);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgReaction_c);
};
#endif