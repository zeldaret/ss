#ifndef D_LYT_DRAW_MARK_H
#define D_LYT_DRAW_MARK_H

#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dLytDrawMark_c {
public:
    dLytDrawMark_c() : mStateMgr(*this) {}
    ~dLytDrawMark_c() {}

    bool build(UNKWORD);
    bool remove();
    bool execute();
    bool draw();

    u8 getField_0x940() const {
        return field_0x940;
    }

    void setField_0x941(u8 v) {
        field_0x941 = v;
    }

private:
    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytDrawMark_c);
    /* 0x018 */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnm[16];

    /* 0x4CC */ u8 _0x4CC[0x504 - 0x4CC];
    
    /* 0x504 */ d2d::ResAccIf_c mResAcc;

    /* 0x874 */ u8 _0x874[0x88C - 0x874];

    /* 0x88C */ mVec2_c field_0x88C[6];
    /* 0x8BC */ mVec2_c field_0x8BC[6];

    /* 0x8EC */ u8 _0x8EC[0x940 - 0x8EC];

    /* 0x940 */ u8 field_0x940;
    /* 0x941 */ u8 field_0x941;

    /* 0x942 */ u8 _0x942[0x958 - 0x942];
};

#endif
