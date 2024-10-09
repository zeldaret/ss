#ifndef D_T_GROUP_TEST_H
#define D_T_GROUP_TEST_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgGroupTest_c : public dAcBase_c { 
public:
	dTgGroupTest_c() : mStateMgr(*this, sStateID::null) {}
	virtual ~dTgGroupTest_c() {}

	STATE_FUNC_DECLARE(dTgGroupTest_c, Wait);
	STATE_FUNC_DECLARE(dTgGroupTest_c, CreateLeft);
	STATE_FUNC_DECLARE(dTgGroupTest_c, Fight);
	STATE_FUNC_DECLARE(dTgGroupTest_c, DoNothing);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTgGroupTest_c);
};

#endif
