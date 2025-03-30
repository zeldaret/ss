#ifndef D_LYT_TEXTBOX_H
#define D_LYT_TEXTBOX_H

#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_textBox.h"
#include "nw4r/lyt/lyt_types.h"

class dTextBox_c : public nw4r::lyt::TextBox {
    friend class dWindow_c;

public:
    dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet);

    f32 GetLineWidth(f32 *pOutSpacing);

    void setLytBase(d2d::LytBase_c *lytBase) {
        mpLytBase = lytBase;
    }

    void SetScale(f32 scale) {
        nw4r::lyt::Size value(mTextScale);
        value.width *= scale;
        value.height *= scale;
        mScale = scale;
        MySetScale(value);
        nw4r::lyt::TextBox::SetFontSize(value);
    }

    void set0x1F8(u8 val) {
        field_0x1F8 = val;
    }

    f32 getMyScale() const {
        return mScale;
    }

    void fn_800E0A60(const char *area, ...) {
        // TODO
    }

    void fn_800AF930(const wchar_t *);

    // @bug: This does not implement UT's RTTI, so casts to dTextBox_c will
    // succeed even if all you have is a lyt::TextBox
private:
    void MySetScale(const nw4r::lyt::Size &value);

    /* 0x104 */ d2d::LytBase_c *mpLytBase;
    /* 0x108 */ u8 field_0x108[0x118 - 0x108];
    /* 0x118 */ nw4r::lyt::Size mTextScale;
    /* 0x120 */ f32 mScale;
    /* 0x124 */ u8 field_0x124[0x1F8 - 0x124];
    /* 0x1F8 */ u8 field_0x1F8;
    /* 0x1F9 */ u8 field_0x1F9[0x204 - 0x1F9];
};

#endif
