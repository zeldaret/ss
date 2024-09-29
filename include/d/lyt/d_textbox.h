#ifndef D_LYT_TEXTBOX_H
#define D_LYT_TEXTBOX_H

#include <nw4r/lyt/lyt_textBox.h>

class dTextBox_c : public nw4r::lyt::TextBox {
    friend class dWindow_c;

public:
    dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet);

    f32 calcWidthMaybe(f32 *);

private:
    u8 field_0x104[0x204 - 0x104];
};

#endif
