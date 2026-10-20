#ifndef D_A_OBJ_STAGE_KRAKEN_H
#define D_A_OBJ_STAGE_KRAKEN_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOstageKraken_c : public dAcObjBase_c {
public:
    dAcOstageKraken_c() : mStateMgr(*this) {}
    virtual ~dAcOstageKraken_c() {}

    static dAcOstageKraken_c *GetInstance();

    static dAcOstageKraken_c *sInstance;

    STATE_FUNC_DECLARE(dAcOstageKraken_c, Wait);
    STATE_FUNC_DECLARE(dAcOstageKraken_c, MarkLight);
    STATE_FUNC_DECLARE(dAcOstageKraken_c, AroundMarkLight);
    STATE_FUNC_DECLARE(dAcOstageKraken_c, MarkVanish);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstageKraken_c);

public:
    u8 pad[0x16C4];
    /* 0x1A30 */ u8 field_0x1A30;
    /* 0x1A31 */ u8 mMarkLeftRight;
    /* 0x1A32 */ u8 field_0x1A32;
};

#endif
