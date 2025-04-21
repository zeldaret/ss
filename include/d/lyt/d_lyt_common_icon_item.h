#ifndef D_LYT_COMMON_ICON_ITEM_H
#define D_LYT_COMMON_ICON_ITEM_H

#include "common.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

class dLytCommonIconItemPartBase_c {
public:
    virtual ~dLytCommonIconItemPartBase_c() {}
    virtual nw4r::lyt::Pane *getPane() = 0;
    virtual d2d::LytBase_c *getLyt() = 0;
    virtual const char *getName() const = 0;
};

/** B-Wheel item */
class dLytCommonIconItemPart1_c : public dLytCommonIconItemPartBase_c {
public:
    dLytCommonIconItemPart1_c()
        : mItem(0), mColor(0), field_0x2B4(false), mHasNumber(false), mSize(false), mBocoburinLocked(false) {}
    virtual ~dLytCommonIconItemPart1_c() {}
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void reset();

    bool isCursorOver() const;
    void setNumber(s32 number);
    void setVisible(bool visible);
    void setItem(u8 item);
    void setHasNumber(bool hasNumber) {
        mHasNumber = hasNumber;
    }
    void setUnk(bool unk) {
        field_0x2B4 = unk;
    }
    void setSize(bool size) {
        mSize = size;
    }
    void setBocoburinLocked(bool locked) {
        mBocoburinLocked = locked;
    }
    void setNumberColor(u8 color);
    void setOn();
    void setOff();
    void startConfirm();
    bool isDoneDeciding() const;

private:
    void realizeUnk();
    void realizeNumberV();
    void realizeSize();
    void realizeBocoburin();
    void realizeItem(u8 item);

    /* 0x004 */ d2d::dLytSub mLyt;
    /* 0x098 */ d2d::AnmGroup_c mAnm[7];
    /* 0x258 */ dTextBox_c *mpTextBoxes[2];
    /* 0x260 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x264 */ dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x28C */ u8 mItem;
    /* 0x28D */ u8 mColor;
    /* 0x28E */ u8 _0x28E[0x294 - 0x28E];
    /* 0x294 */ wchar_t mNumberBuf[16];
    /* 0x2B4 */ bool field_0x2B4;
    /* 0x2B5 */ bool mHasNumber;
    /* 0x2B6 */ bool mSize;
    /* 0x2B7 */ bool mBocoburinLocked;
};

/** Pouch item */
class dLytCommonIconItemPart2_c : public dLytCommonIconItemPartBase_c {
public:
    dLytCommonIconItemPart2_c()
        : mItem(0),
          mColor(0),
          field_0x374(0),
          mHasNumber(false),
          mSize(false),
          mBocoburinLocked(false),
          mOnOff(true),
          mDurability(0.0f) {}
    virtual ~dLytCommonIconItemPart2_c() {}
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void reset();

    bool isCursorOver() const;
    void setNumber(s32 number);
    u8 getShieldType() const;
    void setVisible(bool visible);
    void setItem(u8 item);
    void setHasNumber(bool hasNumber) {
        mHasNumber = hasNumber;
    }
    void setUnk(bool unk) {
        field_0x374 = unk;
    }
    void setSize(bool size) {
        mSize = size;
    }
    void setBocoburinLocked(bool locked) {
        mBocoburinLocked = locked;
    }
    void setNumberColor(u8 color);
    void setOn();
    void setOff();
    void startConfirm();
    bool isDoneDeciding() const;
    void setShieldOnOff(bool onOff);

    void setDurability(f32 durability);

private:
    void realizeUnk();
    void realizeNumberV();
    void realizeSize();
    void realizeBocoburin();
    void realizeItem(u8 item);
    void realizeShieldOnOff(bool onOff);
    void realizeShieldType(u8 type);

    /* 0x004 */ d2d::dLytSub mLyt;
    /* 0x098 */ d2d::AnmGroup_c mAnm[10];
    /* 0x318 */ dTextBox_c *mpTextBoxes[2];
    /* 0x320 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x324 */ dCursorHitCheckLyt_c structD;
    /* 0x34C */ u8 mItem;
    /* 0x34D */ u8 mColor;
    /* 0x350 */ u32 mNumber;
    /* 0x354 */ wchar_t mNumberBuf[16];
    /* 0x374 */ u8 field_0x374;
    /* 0x375 */ bool mHasNumber;
    /* 0x376 */ bool mSize;
    /* 0x377 */ bool mBocoburinLocked;
    /* 0x378 */ bool mOnOff;
    /* 0x37C */ f32 mDurability;
};

/** Dowsing "item" */
class dLytCommonIconItemPart3_c : public dLytCommonIconItemPartBase_c {
public:
    dLytCommonIconItemPart3_c() : mItem(0), mSize(false), mBocoburinLocked(false) {}
    virtual ~dLytCommonIconItemPart3_c() {}
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void reset();

    void setSize(bool size) {
        mSize = size;
    }
    void setBocoburinLocked(bool locked) {
        mBocoburinLocked = locked;
    }

    bool isCursorOver() const;
    void setVisible(bool visible);
    void setItem(u8 item);

private:
    void realizeSize();
    void realizeBocoburin();
    void realizeItem(u8 item);

    /* 0x004 */ d2d::dLytSub mLyt;
    /* 0x098 */ d2d::AnmGroup_c mAnm[3];
    /* 0x158 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x15C */ dCursorHitCheckLyt_c structD;
    /* 0x184 */ u8 mItem;
    /* 0x185 */ bool mSize;
    /* 0x186 */ bool mBocoburinLocked;
};

/**
 * A common item component used in various UI screens like the item check, scrap shop,
 * pause menu wheels, B item preview in the top right HUD corner.
 * NOT used for the HUD wheels.
 */
class dLytCommonIconItem_c : public d2d::dSubPane {
public:
    dLytCommonIconItem_c() : mStateMgr(*this, sStateID::null), mPart(3) {}
    ~dLytCommonIconItem_c() {}

    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        switch (mPart) {
            case 0:  return mPart1.getPane();
            case 1:  return mPart2.getPane();
            case 2:  return mPart3.getPane();
            default: return nullptr;
        }
    }
    virtual d2d::LytBase_c *getLyt() override {
        switch (mPart) {
            case 0:  return mPart1.getLyt();
            case 1:  return mPart2.getLyt();
            case 2:  return mPart3.getLyt();
            default: return nullptr;
        }
    }
    virtual const char *getName() const override {
        switch (mPart) {
            case 0:  return mPart1.getName();
            case 1:  return mPart2.getName();
            case 2:  return mPart3.getName();
            default: return nullptr;
        }
    }

    bool build(d2d::ResAccIf_c *resAcc, u8 variant);

    void reset();
    void setNumber(s32 number);
    bool isCursorOver() const;
    void setVisible(bool visible);
    void setUnk(bool unk);
    void setSize(bool size);
    void setBocoburinLocked(bool locked);
    void setHasNumber(bool hasNumber);
    void setItem(u8 item);
    /** 0 -> golden, 1 -> red, 2 -> green */
    void setNumberColor(u8 color);
    /** Increases item size, e.g. when hovering over item in Item Check */
    void setOn();
    /** Restores item size to normal */
    void setOff();

    /** The "decide" animation in the scrap shop makes the selected item flash for a
    short time before showing the upgrade screen */
    void startConfirm();
    bool isDoneDeciding() const;

    void setShieldOnOff(bool onOff);
    void setShieldDurability(f32 durability);

private:
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, None);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, In);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonIconItem_c, Out);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytCommonIconItem_c);
    /* 0x044 */ u8 mPart;
    /* 0x048 */ dLytCommonIconItemPart1_c mPart1;
    /* 0x300 */ dLytCommonIconItemPart2_c mPart2;
    /* 0x680 */ dLytCommonIconItemPart3_c mPart3;
};

#endif
