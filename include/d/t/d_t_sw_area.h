#ifndef D_T_SW_AREA_H
#define D_T_SW_AREA_H

#include "d/d_area.h"
#include "d/t/d_tg.h"

class dTgSwArea_c : public dTg_c {
public:
    dTgSwArea_c() {}
    virtual ~dTgSwArea_c() {}

    virtual int create() override;
    virtual int actorExecute() override;

private:
    dArea_c mArea;
    u8 mSetSceneflag;
    u8 mUnsetSceneflag;
    bool mbTemporary;
    f32 mScaleModify;
    u16 mSetStoryflag;
    u16 mUnsetStoryflag;
};

#endif
