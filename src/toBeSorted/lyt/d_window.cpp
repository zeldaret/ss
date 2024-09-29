#include <toBeSorted/lyt/d_textbox.h>
#include <toBeSorted/lyt/d_window.h>


dWindow_c::dWindow_c(const nw4r::lyt::res::Window *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet)
    : nw4r::lyt::Window(pBlock, ResBlockSet) {}

void dWindow_c::fn_800B1670(dTextBox_c *textBox, f32 f) {
    if (textBox == nullptr) {
        return;
    }

    u8 rem = mBasePosition % 3;
    if (rem == 0) {
        nw4r::math::VEC3 other = textBox->GetTranslate();
        nw4r::math::VEC3 my = GetTranslate();
        f = (other.x - my.x) * 2.0f;
    } else if (rem == 2) {
        nw4r::math::VEC3 other = textBox->GetTranslate();
        nw4r::math::VEC3 my = GetTranslate();
        f = (my.x - other.x) * 2.0f;
    }

    nw4r::lyt::Size size = *GetSize();
    f32 tmp = textBox->calcWidthMaybe(nullptr);
    tmp += f;
    size.width = tmp;
    SetSize(size);
}
