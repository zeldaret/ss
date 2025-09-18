#ifndef D_A_OBJ_LOG_H
#define D_A_OBJ_LOG_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOlog_c : public dAcObjBase_c {
public:
    dAcOlog_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOlog_c() {}

    STATE_FUNC_DECLARE(dAcOlog_c, Wait);
    STATE_FUNC_DECLARE(dAcOlog_c, Move);
    STATE_FUNC_DECLARE(dAcOlog_c, MoveEnd);
    STATE_FUNC_DECLARE(dAcOlog_c, Fall);
    STATE_FUNC_DECLARE(dAcOlog_c, Shake);

    u8 getField_0xE4E() const {
        return field_0xE4E;
    }

private:
    // TODO: Fixup stuff
    /* 0x330 */ u8 _330[0xE4E - 0x330];
    /* 0xE4E */ u8 field_0xE4E;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOlog_c);
};

#endif
