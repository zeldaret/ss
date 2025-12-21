#ifndef D_LYT_DEPOSIT_BOX_CURSOR_H
#define D_LYT_DEPOSIT_BOX_CURSOR_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "nw4r/lyt/lyt_pane.h"

class dLytDepositBoxCursor_c : public d2d::dSubPane {
public:
    enum ItemLocation_e {
        LOC_POUCH,
        LOC_DEPOSIT,
    };

public:
    /* vt 0x0C */ virtual bool build(d2d::ResAccIf_c *resAcc) override;
    /* vt 0x10 */ virtual bool remove() override;
    /* vt 0x14 */ virtual bool execute() override;
    /* vt 0x18 */ virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    /* vt 0x1C */ virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    /* vt 0x20 */ virtual const char *getName() const override {
        return mLyt.getName();
    }

    /* vt 0x08 */ virtual ~dLytDepositBoxCursor_c() {}

    bool draw();

    void init(s32 location, s32 slot, s32 item, bool unk);
    void updateSlot(s32 location, s32 slot, s32 unk, bool unk2);
    void fn_801580A0();

private:
    void initIcon();
    void realizeNav();

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[2];
    /* 0x11C */ nw4r::lyt::Pane *mpStickCursorPane;
    /* 0x120 */ nw4r::lyt::Pane *mpGetIconPane;
    /* 0x124 */ dLytCommonIconItem_c mIcon;
    /* 0x930 */ d2d::SubPaneList mSubpaneList;
    /* 0x938 */ d2d::SubPaneListNode mNode;
    /* 0x948 */ bool mIsVisible;
    /* 0x948 */ bool field_0x949;
    /* 0x94C */ s32 mSlot;
    /* 0x950 */ s32 field_0x950;
    /* 0x954 */ s32 mItemLocation;
};

#endif
