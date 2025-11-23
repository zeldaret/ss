#ifndef D_LYT_DRAW_MARK_H
#define D_LYT_DRAW_MARK_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

/** 2D UI - Handles drawing the shape for re-sealing the Imprisoned One */
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
    void buildVariant01();
    void buildVariant02();
    void buildVariant03();

    STATE_FUNC_DECLARE(dLytDrawMark_c, Wait);
    STATE_FUNC_DECLARE(dLytDrawMark_c, In);
    STATE_FUNC_DECLARE(dLytDrawMark_c, RingIn);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveWait);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveDraw);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveFix);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveEnd);
    STATE_FUNC_DECLARE(dLytDrawMark_c, Complete);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytDrawMark_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnm[16];

    /* 0x4CC */ UNKWORD field_0x4CC;

    /* 0x4D0 */ u8 _0x4D0[0x500 - 0x4D0];

    /* 0x500 */ UNKWORD field_0x500;

    /* 0x504 */ d2d::ResAccIf_c mResAcc;

    /* 0x874 */ mVec2_c field_0x874;
    /* 0x87C */ mVec2_c field_0x87C;

    /* 0x888 */ u8 _0x888[0x88C - 0x884];

    /* 0x88C */ mVec2_c field_0x88C[6];
    /* 0x8BC */ mVec2_c field_0x8BC[6];

    /* 0x8EC */ UNKWORD field_0x8EC;

    /* 0x8F0 */ s32 mVariant;
    /* 0x8F4 */ UNKWORD field_0x8F4;
    /* 0x8F8 */ UNKWORD field_0x8F8;
    /* 0x8FC */ UNKWORD field_0x8FC;
    /* 0x900 */ UNKWORD field_0x900;
    /* 0x904 */ UNKWORD field_0x904;
    /* 0x908 */ UNKWORD field_0x908;

    /* 0x90C */ u8 _0x90C[0x910 - 0x90C];

    /* 0x910 */ f32 field_0x910[12];

    /* 0x940 */ u8 field_0x940;
    /* 0x941 */ u8 field_0x941;
    /* 0x942 */ u8 field_0x942;
    /* 0x943 */ u8 field_0x943;
    /* 0x944 */ u8 field_0x944;

    /* 0x945 */ u8 _0x945[0x955 - 0x945];

    /* 0x955 */ u8 field_0x955;
    /* 0x956 */ u8 field_0x956;
    /* 0x957 */ u8 field_0x957;
};

#endif
