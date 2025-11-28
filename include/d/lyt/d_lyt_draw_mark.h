#ifndef D_LYT_DRAW_MARK_H
#define D_LYT_DRAW_MARK_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
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

    void setForceComplete(bool v) {
        mForceComplete = v;
    }

private:
    enum Variant_e {
        VAR_IMP_1,
        VAR_IMP_2,
        VAR_IMP_3,
    };

    void buildVariant01();
    void buildVariant02();
    void buildVariant03();

    void convertScreenPosToDpdPos(mVec2_c *pOutDpd, mVec2_c *screenPos) const;
    void fn_800ADC10(mVec2_c screenPos) const;
    s32 getNumLines() const;
    void loadPanePos();
    void detectSwordSwing();

    STATE_FUNC_DECLARE(dLytDrawMark_c, Wait);
    STATE_FUNC_DECLARE(dLytDrawMark_c, In);
    STATE_FUNC_DECLARE(dLytDrawMark_c, RingIn);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveWait);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveDraw);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveFix);
    STATE_FUNC_DECLARE(dLytDrawMark_c, MoveEnd); // Unused?
    STATE_FUNC_DECLARE(dLytDrawMark_c, Complete);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytDrawMark_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0CC */ d2d::AnmGroup_c mAnm[16];

    /* 0x4CC */ nw4r::lyt::Pane *mpPanes[13];
    /* 0x500 */ nw4r::lyt::Pane *field_0x500;

    /* 0x504 */ d2d::ResAccIf_c mResAcc;

    /* 0x874 */ mVec2_c field_0x874;
    /* 0x87C */ mVec2_c mDpdPosLastFrame;
    /* 0x884 */ mVec2_c mDpdPosScreen;

    /* 0x88C */ mVec2_c mStartPositions[6];
    /* 0x8BC */ mVec2_c mEndPositions[6];

    /* 0x8EC */ s32 mLineLoopIndex;

    /* 0x8F0 */ s32 mVariant;
    /* 0x8F4 */ s32 mSavedLineLoopIndex;
    /* 0x8F8 */ s32 field_0x8F8;
    /* 0x8FC */ s32 mLineFixIndex;
    /* 0x900 */ s32 field_0x900;
    /* 0x904 */ s32 mDpdPosInitTimer;
    /* 0x908 */ s32 mSampleTimer;

    /* 0x90C */ u8 _0x90C[0x910 - 0x90C];

    /* 0x910 */ f32 field_0x910;
    /* 0x914 */ f32 field_0x914;
    /* 0x918 */ f32 field_0x918[10];

    /* 0x940 */ bool field_0x940;
    /* 0x941 */ bool mForceComplete;
    /* 0x942 */ bool mLastLine;
    /* 0x943 */ bool mNeedEnableLineLoop;
    /* 0x944 */ u8 field_0x944;

    /* 0x945 */ bool mAnmCtrlInUse[16];

    /* 0x955 */ bool field_0x955;
    /* 0x956 */ bool field_0x956;
    /* 0x957 */ bool field_0x957;
};

#endif
