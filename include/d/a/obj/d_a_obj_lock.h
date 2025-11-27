#ifndef D_A_OBJ_LOCK_H
#define D_A_OBJ_LOCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOLock_c : public dAcObjBase_c {
public:
    dAcOLock_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOLock_c() {}

    STATE_FUNC_DECLARE(dAcOLock_c, Wait);

    bool checkField_0x2091() const {
        return field_0x2091;
    }
    void setField_0x2090() {
        field_0x2090 = true;
    }

private:
    /* 0x???? */ STATE_MGR_DECLARE(dAcOLock_c);
    /* 0x???? */ u8 _0x36C[0x2090 - 0x36C];
    /* 0x2090 */ bool field_0x2090;
    /* 0x2091 */ bool field_0x2091;
};

#endif
