#ifndef D_T_MAP_MARK_H
#define D_T_MAP_MARK_H

#include "d/t/d_tg.h"

class dTgMapMark_c : public dTg_c {
public:
    dTgMapMark_c() {}
    virtual ~dTgMapMark_c() {}

    /* 0x0FC */ u16 mTriggerStoryFlag;
    /* 0x0FE */ u16 rotz_shift8_0xff;
    /* 0x100 */ u8 shift4_0x8;
    /* 0x101 */ u8 shift23_0x8;
    /* 0x102 */ u8 shift0_0x3;
    /* 0x103 */ u8 shift2_0x3;
    /* 0x104 */ u8 shift4_0x3f;
    /* 0x105 */ u8 rotz_shift0_0xff;
    /* 0x108 */ s32 rotz_shift0_0xf;
    /* 0x10C */ bool field_0x10C;
    /* 0x10D */ bool field_0x10D;
    /* 0x10E */ bool field_0x10E;
    /* 0x10F */ u8 rotz_shift10_0x3;
    /* 0x110 */ u8 rotz_shift12_0x3;
};

#endif
