#ifndef D_CAMERA_H
#define D_CAMERA_H

#include "d/d_base.h"

extern "C" bool fn_80081FE0(void*, const char *);

class dCamera_c : public dBase_c {
public:
    /* 0x068 */ u8 _0x068[0xD98 - 0x068];
    /* 0xD98 */ void *field_0xD98;

    bool isCurrentTrend(const char *name) const {
        return fn_80081FE0(field_0xD98, name);
    }
};

#endif
