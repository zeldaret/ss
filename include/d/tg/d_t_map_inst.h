#ifndef D_T_MAP_INST_H
#define D_T_MAP_INST_H

#include <d/tg/d_tg.h>

class dTgMapInst_c : public dTg_c {
public:
    dTgMapInst_c() {}
    virtual ~dTgMapInst_c() {}

    virtual int create() override;

    u8 field_0xfc;
};

#endif
