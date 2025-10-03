#ifndef D_LYT_MAP_MARKERS_H
#define D_LYT_MAP_MARKERS_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "sized_string.h"

/** 2D UI - Map - Popup text that appears when pointing at certain points of interest */
class dLytMapPopup_c {
public:
    dLytMapPopup_c() {}
    virtual ~dLytMapPopup_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void draw();

    void setLabel(const char *label);
    void setInout(f32 value);
    f32 getTextRenderWidth() const;

private:
    enum ModeCheck_e {
        MODE_TRANSITION_TO_ZOOM = 2,
        MODE_TRANSITION_TO_STAGE = 3,
        MODE_STABLE_STAGE = 4,
        MODE_STABLE_ZOOM = 5,
    };

    void realizeText();
    void setInitialState();

    void checkMapMode();
    void updateScale();

    /* 0x004 */ d2d::LytBase_c mLyt;
    /* 0x094 */ d2d::AnmGroup_c mAnm[2];
    /* 0x114 */ SizedString<0x40> mLabel;
    /* 0x154 */ nw4r::lyt::Pane *mpPanes[2];
    /* 0x15C */ dTextBox_c *mpTextboxes[2];
    /* 0x164 */ dWindow_c *mpWindow[1];
    /* 0x168 */ dTextBox_c *mpSizeBox[1];
    /* 0x16C */ s32 mModeCheckResult;
    /* 0x170 */ mVec2_c mPosition;
    /* 0x178 */ u8 field_0x178;
    /* 0x17C */ f32 mMaxScale;
    /* 0x180 */ UNKWORD field_0x180;
};

#endif
