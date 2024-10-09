#ifndef D_A_INSECT_BEETLE_H
#define D_A_INSECT_BEETLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcInsectBeetle_c : public dAcObjBase_c {
public:
    dAcInsectBeetle_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcInsectBeetle_c() {}

    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Wait);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Walk);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Escape);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Fly);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Fall);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Getup);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Finalize);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, Dead);
    STATE_FUNC_DECLARE(dAcInsectBeetle_c, EscapeChild);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcInsectBeetle_c);
};

#endif
