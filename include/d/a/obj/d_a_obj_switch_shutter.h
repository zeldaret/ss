#ifndef D_A_OBJ_SWITCH_SHUTTER_H
#define D_A_OBJ_SWITCH_SHUTTER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwShutter_c : public dAcObjBase_c {
public:
    dAcOSwShutter_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSwShutter_c() {}

    STATE_FUNC_DECLARE(dAcOSwShutter_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwShutter_c, Move);
    STATE_FUNC_DECLARE(dAcOSwShutter_c, WaitEventEnd);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwShutter_c);
};

#endif
