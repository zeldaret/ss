#ifndef D_T_LIGHT_SHAFT_EFF_H
#define D_T_LIGHT_SHAFT_EFF_H

#include "d/t/d_tg.h"

class dTgLightShaftEff_c : public dTg_c {
public:
    dTgLightShaftEff_c() {}
    virtual ~dTgLightShaftEff_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;
};

#endif
