#ifndef D_T_HEAT_RESIST_H
#define D_T_HEAT_RESIST_H

#include "common.h"
#include "d/d_area.h"
#include "d/t/d_tg.h"

class dTgHeatResist_c : public dTg_c {
public:
    dTgHeatResist_c() {}
    virtual ~dTgHeatResist_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

private:
    /* 0x0FC */ dArea_c mArea;
    /* 0x12C */ u8 mInverted;
};

#endif
