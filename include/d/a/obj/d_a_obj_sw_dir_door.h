#ifndef D_A_OBJ_SW_DIR_DOOR_H
#define D_A_OBJ_SW_DIR_DOOR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwDirDoor_c : public dAcObjBase_c {
public:
    dAcOSwDirDoor_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSwDirDoor_c() {}

    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, CorrectStart);
    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, Correct);
    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, Broken);
    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, BrokenEnd);
    STATE_FUNC_DECLARE(dAcOSwDirDoor_c, DemoEnd);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwDirDoor_c);
};

#endif
