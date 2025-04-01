#ifndef D_LYT_MGS_WINDOW_COMMON_H
#define D_LYT_MGS_WINDOW_COMMON_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "m/m2d.h"

class dLytMsgWindowSubtype : public m2d::Base_c {
public:
    /* vt 0x08 */ virtual ~dLytMsgWindowSubtype() {}
    // vt 0x0C = m2d::Base_c::draw
    /* vt 0x10 */ virtual bool build(void *unk, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor);
    /* vt 0x14 */ virtual bool remove();
    /* vt 0x18 */ virtual bool execute();
    /* vt 0x1C */ virtual void vt_0x1C();
    /* vt 0x20 */ virtual bool vt_0x20() const;
    /* vt 0x24 */ virtual void vt_0x24();
    /* vt 0x28 */ virtual bool vt_0x28() const;
    /* vt 0x2C */ virtual bool setText(const wchar_t *text);
    /* vt 0x30 */ virtual dTextBox_c *vt_0x30();
    /* vt 0x34 */ virtual void vt_0x34();
    /* vt 0x38 */ virtual void vt_0x38(bool b);
    /* vt 0x3C */ virtual u8 vt_0x3C() const;
    /* vt 0x40 */ virtual bool vt_0x40() const;
};

#endif
