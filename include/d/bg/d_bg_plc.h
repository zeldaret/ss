#ifndef D_BG_D_BG_PLC_H
#define D_BG_D_BG_PLC_H

#include "d/bg/d_bg_pc.h"

enum {
    ZELDA_CODE_SIZE = 0x14
};

struct PLC {
    /* 0x0 */ u32 magic;
    /* 0x4 */ u16 m_code_size;
    /* 0x6 */ u16 m_num;
    /* 0x8 */ sBgPc field_0x8[0];
};

class dBgPlc {
public:
    dBgPlc();
    ~dBgPlc();
    void setBase(void *);
    sBgPc *getCode(int, sBgPc **) const;
    u32 getGrpCode(int) const;

private:
    /* 0x00 */ PLC *m_base;
};

#endif
