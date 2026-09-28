#ifndef D_A_B_GIRAHIMU2_H
#define D_A_B_GIRAHIMU2_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcGirahimu2_c : public dAcEnBase_c {
public:
    dAcGirahimu2_c() : mStateMgr(*this) {}
    virtual ~dAcGirahimu2_c() {}

    void setField_0xD52(u16 val) {
        field_0xD52 = val;
    }

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu2_c);

    /* 0xD52 */ u16 field_0xD52;
};

#endif
