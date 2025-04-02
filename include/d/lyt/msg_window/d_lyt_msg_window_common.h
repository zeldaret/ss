#ifndef D_LYT_MGS_WINDOW_COMMON_H
#define D_LYT_MGS_WINDOW_COMMON_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "m/m2d.h"

class dLytMsgWindowSubtype : public m2d::Base_c {
public:
    /* vt 0x08 */ virtual ~dLytMsgWindowSubtype() {}
    // vt 0x0C = m2d::Base_c::draw
    /* vt 0x10 */ virtual bool build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor);
    /* vt 0x14 */ virtual bool remove();
    /* vt 0x18 */ virtual bool execute();
    /* vt 0x1C */ virtual void open();
    /* vt 0x20 */ virtual bool isOpening() const;
    /* vt 0x24 */ virtual void close();
    /* vt 0x28 */ virtual bool isClosing() const;
    /* vt 0x2C */ virtual bool setText(const wchar_t *text);
    /* vt 0x30 */ virtual dTextBox_c *vt_0x30();
    /* vt 0x34 */ virtual bool vt_0x34() {
        return true;
    }
    /* vt 0x38 */ virtual bool vt_0x38(bool b) {
        return true;
    }
    /* vt 0x3C */ virtual bool vt_0x3C() const {
        return true;
    }
    /* vt 0x40 */ virtual bool vt_0x40() const;
};

#endif
