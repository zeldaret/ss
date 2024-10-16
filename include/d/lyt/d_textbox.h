#ifndef D_LYT_TEXTBOX_H
#define D_LYT_TEXTBOX_H

#include <d/lyt/d2d.h>
#include <nw4r/lyt/lyt_textBox.h>


extern "C" void *lbl_805753B0;

class dTextBox_c : public nw4r::lyt::TextBox {
    friend class dWindow_c;

public:
    dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet);

    f32 GetLineWidth(f32 *pOutSpacing);

    void setLytBase(d2d::LytBase_c *lytBase) {
        mpLytBase = lytBase;
    }

    void SetScale(float scale) {
        nw4r::math::VEC2 value = GetScale();
        value.x = GetScale().x * scale;
        value.y = GetScale().y * scale;
        mScale = scale;
        MySetScale(value);
        nw4r::lyt::TextBox::SetScale(value);
    }


    static inline f32 GetTranslateX1() {
        if (lbl_805753B0 != nullptr) {
            return GetTranslateX1_();
        } else {
            return 0.0f;
        }
    }

    void fn_800E0A60(const char *area, ...) {
        // TODO
    }

    static f32 GetTranslateX1_();

    // @bug: This does not implement UT's RTTI, so casts to dTextBox_c will
    // succeed even if all you have is a lyt::TextBox
private:

    void MySetScale(const nw4r::math::VEC2 &value);

    d2d::LytBase_c *mpLytBase;
    u8 field_0x108[0x118 - 0x108];
    /* 0x118 */ nw4r::math::VEC2 mTextScale;
    /* 0x120 */ f32 mScale;
    u8 field_0x124[0x204 - 0x124];
};

#endif
