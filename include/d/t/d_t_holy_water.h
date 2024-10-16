#ifndef D_T_HOLY_WATER_H
#define D_T_HOLY_WATER_H

#include "d/t/d_tg.h"
#include "toBeSorted/dowsing_target.h"

class dTgHolyWater_c : public dTg_c {
public:
    dTgHolyWater_c() : mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dTgHolyWater_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool isLinkNearby();
    bool dowsingActive();

private:
    DowsingTarget mDowsingTarget; // 0x0FC
};

#endif
