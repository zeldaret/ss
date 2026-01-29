#ifndef D_LYT_COMMON_ICON_MATERIAL_H
#define D_LYT_COMMON_ICON_MATERIAL_H

#include "common.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

/** Material - Bug */
class dLytCommonIconMaterialPart1_c {
public:
    dLytCommonIconMaterialPart1_c() {}
    virtual ~dLytCommonIconMaterialPart1_c() {}
    virtual nw4r::lyt::Pane *getPane() {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() {
        return &mLyt;
    }
    virtual const char *getName() const {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void reset();
    void setItem(u8 item);

    void setNumber(s32 number);
    void setOn();
    void setOff();

    bool isCursorOver() const;
    void setVisible(bool visible);

    const nw4r::lyt::Bounding *getBounding() const {
        return mpBounding;
    }

    void setBg(bool bg) {
        mBg = bg;
    }
    void setHasNumber(bool hasNumber) {
        mNumberV = hasNumber;
    }
    void setShadow(bool shadow) {
        mShadow = shadow;
    }

private:
    void realizeBg();
    void realizeNumberV();
    void realizeShadow();
    void realizeItem(u8 item);

    /* 0x004 */ d2d::dLytSub mLyt;
    /* 0x098 */ d2d::AnmGroup_c mAnm[6];
    /* 0x218 */ dTextBox_c *mpTextBoxes[2];
    /* 0x220 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x224 */ dCursorHitCheckLyt_c mCursor;
    /* 0x24C */ u8 mItem;
    /* 0x24D */ u8 _0x24D[0x254 - 0x24D];
    /* 0x254 */ wchar_t mNumberBuf[16];
    /* 0x274 */ bool mBg;
    /* 0x275 */ bool mNumberV;
    /* 0x276 */ bool mShadow;
};

/** Material - Treasure */
class dLytCommonIconMaterialPart2_c {
public:
    dLytCommonIconMaterialPart2_c() {}
    virtual ~dLytCommonIconMaterialPart2_c() {}
    virtual nw4r::lyt::Pane *getPane() {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() {
        return &mLyt;
    }
    virtual const char *getName() const {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    void reset();
    void setItem(u8 item);

    void setNumber(s32 number);
    void setOn();
    void setOff();

    bool isCursorOver() const;
    void setVisible(bool visible);

    const nw4r::lyt::Bounding *getBounding() const {
        return mpBounding;
    }

    void setBg(bool bg) {
        mBg = bg;
    }
    void setHasNumber(bool hasNumber) {
        mNumberV = hasNumber;
    }
    void setShadow(bool shadow) {
        mShadow = shadow;
    }

private:
    void realizeBg();
    void realizeNumberV();
    void realizeShadow();
    void realizeItem(u8 item);

    /* 0x004 */ d2d::dLytSub mLyt;
    /* 0x098 */ d2d::AnmGroup_c mAnm[6];
    /* 0x218 */ dTextBox_c *mpTextBoxes[2];
    /* 0x220 */ nw4r::lyt::Bounding *mpBounding;
    /* 0x224 */ dCursorHitCheckLyt_c mCursor;
    /* 0x24C */ u8 mItem;
    /* 0x24D */ u8 _0x24D[0x254 - 0x24D];
    /* 0x254 */ wchar_t mNumberBuf[16];
    /* 0x274 */ bool mBg;
    /* 0x275 */ bool mNumberV;
    /* 0x276 */ bool mShadow;
};

class dLytCommonIconMaterial_c : public d2d::dSubPane {
public:
    dLytCommonIconMaterial_c() : mStateMgr(*this), mPart(2) {}
    ~dLytCommonIconMaterial_c() {}

    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        switch (mPart) {
            case 0:  return mPart1.getPane();
            case 1:  return mPart2.getPane();
            default: return nullptr;
        }
    }
    virtual d2d::LytBase_c *getLyt() override {
        switch (mPart) {
            case 0:  return mPart1.getLyt();
            case 1:  return mPart2.getLyt();
            default: return nullptr;
        }
    }
    virtual const char *getName() const override {
        switch (mPart) {
            case 0:  return mPart1.getName();
            case 1:  return mPart2.getName();
            default: return nullptr;
        }
    }

    bool build(d2d::ResAccIf_c *resAcc, u8 variant);

    void reset();

    void setBg(bool bg);
    void setHasNumber(bool hasNumber);
    void setShadow(bool shadow);
    void setItem(u8 item);
    void setNumber(s32 number);
    void setOn();
    void setOff();

    bool isCursorOver() const;
    void setVisible(bool visible);

    const nw4r::lyt::Bounding *getBounding() const;

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytCommonIconMaterial_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytCommonIconMaterial_c);

private:
    STATE_FUNC_DECLARE(dLytCommonIconMaterial_c, None);
    STATE_FUNC_DECLARE(dLytCommonIconMaterial_c, In);
    STATE_FUNC_DECLARE(dLytCommonIconMaterial_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonIconMaterial_c, Out);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytCommonIconMaterial_c);
    /* 0x044 */ u8 mPart;
    /* 0x048 */ dLytCommonIconMaterialPart1_c mPart1;
    /* 0x0B0 */ dLytCommonIconMaterialPart2_c mPart2;
};

#endif
