#ifndef D_T_GROUP_TEST_H
#define D_T_GROUP_TEST_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgGroupTest_c : public dTg_c {
public:
    dTgGroupTest_c() : mStateMgr(*this) {}
    virtual ~dTgGroupTest_c() {}

    STATE_FUNC_DECLARE(dTgGroupTest_c, Wait);
    STATE_FUNC_DECLARE(dTgGroupTest_c, CreateLeft);
    STATE_FUNC_DECLARE(dTgGroupTest_c, Fight);
    STATE_FUNC_DECLARE(dTgGroupTest_c, DoNothing);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgGroupTest_c);
};

#endif
