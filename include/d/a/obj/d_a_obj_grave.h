#ifndef D_A_OBJ_GRAVE_H
#define D_A_OBJ_GRAVE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOGrave_c : public dAcObjBase_c {
public:
    dAcOGrave_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOGrave_c() {}

    STATE_FUNC_DECLARE(dAcOGrave_c, Wait);
    STATE_FUNC_DECLARE(dAcOGrave_c, Move);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOGrave_c);
};

#endif
