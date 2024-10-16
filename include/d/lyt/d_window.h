#ifndef D_LYT_WINDOW_H
#define D_LYT_WINDOW_H

#include <nw4r/lyt/lyt_window.h>

class dTextBox_c;

class dWindow_c : public nw4r::lyt::Window {
public:
    dWindow_c(const nw4r::lyt::res::Window *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet);
    virtual ~dWindow_c() {}
    void UpdateSize(dTextBox_c *textBox, f32 f);

    // @bug: This does not implement UT's RTTI, so casts to dWindow_c will
    // succeed even if all you have is a lyt::Window

private:
    UNKWORD field_0x108;
};

#endif
