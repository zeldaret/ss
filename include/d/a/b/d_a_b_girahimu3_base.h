#ifndef D_A_B_GIRAHIMU3_BASE_H
#define D_A_B_GIRAHIMU3_BASE_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcGirahimu3Base_c : public dAcEnBase_c { 
public:
	dAcGirahimu3Base_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dAcGirahimu3Base_c() {}

private:
	/* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu3Base_c);
};

#endif
