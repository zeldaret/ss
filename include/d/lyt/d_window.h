#ifndef D_LYT_WINDOW_H
#define D_LYT_WINDOW_H

#include <nw4r/lyt/lyt_window.h>

class dTextBox_c;

class dWindow_c : public nw4r::lyt::Window {
public:
    dWindow_c(const nw4r::lyt::res::Window *pBlock, const nw4r::lyt::ResBlockSet &ResBlockSet);
    virtual ~dWindow_c() {}
    void fn_800B1670(dTextBox_c *textBox, f32 f);

private:
    UNKWORD field_0x108;
};

#endif
