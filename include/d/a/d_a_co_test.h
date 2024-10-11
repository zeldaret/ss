#ifndef D_A_CO_TEST_H
#define D_A_CO_TEST_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcCoTest : public dAcObjBase_c {
public:
    dAcCoTest() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcCoTest() {}

    STATE_FUNC_DECLARE(dAcCoTest, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcCoTest);
};

#endif
