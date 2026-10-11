#ifndef D_A_OBJ_DRUM_H
#define D_A_OBJ_DRUM_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOdrum_c : public dAcObjBase_c {
public:
    dAcOdrum_c() : mStateMgr(*this) {}
    virtual ~dAcOdrum_c() {}

    STATE_FUNC_DECLARE(dAcOdrum_c, Wait);
    STATE_FUNC_DECLARE(dAcOdrum_c, Move);
    STATE_FUNC_DECLARE(dAcOdrum_c, MoveEnd);
    STATE_FUNC_DECLARE(dAcOdrum_c, Fall);
    STATE_FUNC_DECLARE(dAcOdrum_c, Init);
    STATE_FUNC_DECLARE(dAcOdrum_c, BeltMove);
    STATE_FUNC_DECLARE(dAcOdrum_c, TeniN);

    bool getField_0xC48() const {
        return field_0xC48;
    }

private:
    /* 0x330 */ u8 _0x330[0xB7C - 0x330];
    /* 0xB7C */ STATE_MGR_DECLARE(dAcOdrum_c);
    /* 0xBB8 */ u8 _0xBB8[0xC48 - 0xBB8];
    /* 0xC48 */ bool field_0xC48;
};

#endif
