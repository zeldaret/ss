#ifndef D_LYT_SUIRYU_SCORE_H
#define D_LYT_SUIRYU_SCORE_H

#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_note.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

struct dLytNote2_c {
    /* 0x000 */ dLytNote_c mNote;
    /* 0x278 */ UNKWORD field_0x278;
};

class dLytMeterSuiryuScoreBase_c {
public:
    dLytMeterSuiryuScoreBase_c()
        : mStateMgr(*this), field_0x2250(0), field_0x2262(0), field_0x2263(0), field_0x2264(0) {}
    virtual ~dLytMeterSuiryuScoreBase_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    void startIn();
    void startOut();
    void setColor0(u8 idx);
    void setColor1(u8 idx);
    void alphaForwards(s32 idx, f32 rate);
    void alphaBackwards(s32 idx, f32 rate);
    void stopMove();

    void fn_802978F0(s32 idx);
    void fn_80297AE0();
    void fn_80297B40();
    void fn_80297BA0();
    void fn_80297C80();
    bool fn_80297E10(u8) const;
    void fn_80297EA0();
    void fn_80297FE0(u8);
    bool fn_80298B80();

    nw4r::lyt::Pane *getInOutAllPane() const {
        return mLyt.findPane("N_inOutAll_00");
    }

    nw4r::lyt::Pane *getNotePane(s32 idx) const {
        return mpPanes[idx];
    }

    u8 getField_0x2251(s32 idx) const {
        return field_0x2251[idx];
    }

    void setField_0x2251(u8 idx, u8 val) {
        if (field_0x2251[idx] != val) {
            field_0x2251[idx] = val;
        }
    }

    void setField_0x2251_(u8 idx, u8 val) {
        field_0x2251[idx] = val;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterSuiryuScoreBase_c, None);
    STATE_FUNC_DECLARE(dLytMeterSuiryuScoreBase_c, In);
    STATE_FUNC_DECLARE(dLytMeterSuiryuScoreBase_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterSuiryuScoreBase_c, Out);

    void initAnm();
    bool fn_80298600();
    void fn_80298B10();

    /* 0x0004 */ UI_STATE_MGR_DECLARE(dLytMeterSuiryuScoreBase_c);
    /* 0x0040 */ d2d::LytBase_c mLyt;
    /* 0x00D0 */ d2d::AnmGroup_c mAnm[129];
    /* 0x2110 */ nw4r::lyt::Pane *mpPanes[77];
    /* 0x2244 */ nw4r::lyt::Pane *mpPaneInOutAll;
    /* 0x2248 */ mVec2_c mMainTranslate;

    /* 0x2250 */ u8 field_0x2250;
    /* 0x2251 */ u8 field_0x2251[17];

    /* 0x2262 */ u8 field_0x2262;
    /* 0x2263 */ u8 field_0x2263;
    /* 0x2264 */ UNKWORD field_0x2264;
};

class dLytMeterClefMain_c {
private:
    static const s32 NUM_TADTONE_GROUPS = 17;
    static const s32 NUM_TOTAL_TADTONES = 77;

public:
    dLytMeterClefMain_c()
        : mStateMgr(*this),
          field_0x0040(0),
          field_0x5E4C(0),
          field_0x5E4D(0),
          field_0x5F35(0),
          field_0x5F36(0),
          field_0x5F38(0),
          field_0x5F3C(0),
          field_0x5F40(0) {}
    virtual ~dLytMeterClefMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

private:
    void startIn();
    void startOut();
    void fn_80299DF0();
    void fn_80299E30();
    void fn_8029A050();
    void fn_8029A200();
    void fn_8029A260();
    void fn_8029A2C0();
    void fn_8029A3B0();
    void fn_8029A440();

    STATE_FUNC_DECLARE(dLytMeterClefMain_c, None);
    STATE_FUNC_DECLARE(dLytMeterClefMain_c, In);
    STATE_FUNC_DECLARE(dLytMeterClefMain_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterClefMain_c, Out);

    /* 0x0004 */ UI_STATE_MGR_DECLARE(dLytMeterClefMain_c);
    /* 0x0040 */ u8 field_0x0040;
    /* 0x0044 */ dLytMeterSuiryuScoreBase_c mBase;
    /* 0x22AC */ dLytNote2_c mNotes[24];
    /* 0x5E4C */ u8 field_0x5E4C;
    /* 0x5E4D */ u8 field_0x5E4D;

    /* 0x5E4E */ u8 field_0x5E4E[NUM_TADTONE_GROUPS];
    /* 0x5E5F */ u8 field_0x5E5F[NUM_TADTONE_GROUPS];
    /* 0x5E70 */ u8 field_0x5E70[NUM_TADTONE_GROUPS];
    /* 0x5E81 */ u8 field_0x5E81[NUM_TADTONE_GROUPS];
    /* 0x5E92 */ u8 field_0x5E92[NUM_TADTONE_GROUPS];
    /* 0x5EA4 */ UNKWORD field_0x5EA4[NUM_TADTONE_GROUPS];
    /* 0x5EE8 */ u8 field_0x5EE8[NUM_TOTAL_TADTONES];

    /* 0x5F35 */ u8 field_0x5F35;
    /* 0x5F36 */ u8 field_0x5F36;

    /* 0x5F38 */ UNKWORD field_0x5F38;
    /* 0x5F3C */ UNKWORD field_0x5F3C;
    /* 0x5F40 */ UNKWORD field_0x5F40;
};

// Alternative name ideas: dLytMeterClef_c, dLytMeterSuiryuScore_c
// The profile name is LYT_SUIRYU_SCORE so I went with that class name,
// but the fact that it contains a dLytMeterClefMain_c (known name) is suspicious...
class dLytSuiryuScore_c : public dBase_c {
public:
    dLytSuiryuScore_c() {}
    virtual ~dLytSuiryuScore_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

private:
    static dLytSuiryuScore_c *sInstance;

    /* 0x0060 */ d2d::ResAccIf_c mResAcc;
    /* 0x03D8 */ dLytMeterClefMain_c mMain;
};

#endif
