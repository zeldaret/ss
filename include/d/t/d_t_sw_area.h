#ifndef D_T_SW_AREA_H
#define D_T_SW_AREA_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"

class dTgSwArea_c : public dTg_c {
public:
    dTgSwArea_c() {}
    virtual ~dTgSwArea_c() {}

    virtual int create() override;
    virtual int actorExecute() override;

private:
    mMtx_c area;
    u8 setSceneflag;
    u8 unsetSceneflag;
    bool unsetOnLeave;
    f32 scale;
    u16 setStoryflag;
    u16 unsetStoryflag;
};

#endif
