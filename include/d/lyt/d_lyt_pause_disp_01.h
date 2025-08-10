#ifndef LYT_PAUSE_DISP_01_H
#define LYT_PAUSE_DISP_01_H

#include "common.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_material.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytPauseDisp01_c {
public:
    dLytPauseDisp01_c();
    virtual ~dLytPauseDisp01_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void drawDirectly();
    void requestIn(bool scroll);
    void requestOut(bool scroll);
    void requestSelect();
    void requestUnselect();

    STATE_FUNC_DECLARE(dLytPauseDisp01_c, None);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, In);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Select);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Out);

    bool getField_0x98CE() const {
        return field_0x98CE;
    }

private:
    void setAnm(int idx, f32 value);
    void stopAnm(int idx);

    void setupDisp();
    void setupInsects();
    void setupMaterials();

    void showInsectsAndMaterials();
    s32 updateSelection();
    s32 getPointerPane() const;
    void hideInsectsAndMaterials();

    bool shouldInsectBeDisplayed(s32 insectIdx) const;
    u16 getInsectItemId(s32 insectIdx) const;
    s32 getInsectIcon(s32 insectIdx) const;
    s32 getInsectCountByIndex(s32 insectIdx) const;

    u16 getQuestItemId(s32 questItemIndex) const;

    bool shouldMaterialBeDisplayed(s32 materialIdx) const;
    u16 getMaterialItemId(s32 materialIdx) const;
    s32 getMaterialIcon(s32 materialIdx) const;
    s32 getMaterialCountByIndex(s32 materialIdx) const;

    /* 0x0004 */ UI_STATE_MGR_DECLARE(dLytPauseDisp01_c);
    /* 0x0040 */ d2d::LytBase_c mLyt;
    /* 0x00D0 */ d2d::AnmGroup_c mAnm[15];
    /* 0x0490 */ dLytCommonIconMaterial_c mIcons[28];
    /* 0x96B0 */ d2d::SubPaneList mSubpaneList;
    /* 0x96BC */ d2d::SubPaneListNode mSubpanes[28];
    /* 0x987C */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x98A4 */ nw4r::lyt::Pane *mpPanes[6];
    /* 0x98BC */ s32 mStep;
    /* 0x98C0 */ s32 mPrevNavTarget;
    /* 0x98C4 */ s32 mCurrentNavTarget;
    /* 0x98C8 */ s32 mGetDemoTimer;
    /* 0x98CC */ bool mInRequest;
    /* 0x98CD */ bool mOutRequest;
    /* 0x98CE */ bool field_0x98CE;
    /* 0x98CF */ bool mIsVisible;
    /* 0x98D0 */ bool mDoScrollAnim;
    /* 0x98D1 */ bool field_0x98D1;
    /* 0x98D2 */ bool mSelectToggle;
};

#endif
