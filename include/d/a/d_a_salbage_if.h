#ifndef D_A_SALBAGE_IF_H
#define D_A_SALBAGE_IF_H

#include "common.h"

class dSalvageIf_c {
public:
    /* vt 0x08 */ virtual ~dSalvageIf_c() {}
    /* vt 0x0C */ virtual void setCarried() = 0;
    /* vt 0x10 */ virtual void setNotCarried() = 0;
    /* vt 0x14 */ virtual bool isCarried() const = 0;
    /* vt 0x18 */ virtual void setHidden() = 0;
    /* vt 0x1C */ virtual void setNotHidden() = 0;
    /* vt 0x20 */ virtual bool isHidden() const = 0;
    /* vt 0x24 */ virtual s32 getSalvageObjId() const = 0;
    /* vt 0x28 */ virtual void doDemoThrow() = 0;
};

#endif
