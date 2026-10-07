#ifndef D_A_B_GIRAHIMU_H
#define D_A_B_GIRAHIMU_H

#include "d/a/b/d_a_b_girahimu_base.h"
#include "d/a/d_a_base.h"
#include "s/s_State.hpp"

class dAcObjGirahimuKnife_c;

class dAcGirahimu_c : public dAcGirahimuBase_c {
public:
    dAcGirahimu_c() : field_0x220C(false) {}
    virtual ~dAcGirahimu_c() {}

    virtual int doDelete() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorExecute() override;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu_c, dAcGirahimuBase_c, CatchDamage);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu_c, dAcGirahimuBase_c, G_SwordDamage);
    STATE_FUNC_DECLARE(dAcGirahimu_c, G_SwordDemo);
    STATE_FUNC_DECLARE(dAcGirahimu_c, G_SwordWait);
    STATE_FUNC_DECLARE(dAcGirahimu_c, G_SwordPiyori);
    STATE_FUNC_DECLARE(dAcGirahimu_c, Knife);
    STATE_FUNC_DECLARE(dAcGirahimu_c, KnifeAttack);
    STATE_FUNC_DECLARE(dAcGirahimu_c, Death);
    STATE_FUNC_DECLARE(dAcGirahimu_c, DeathMiniGame);

    virtual void vt_0x1DC() override;
    virtual bool vt_0x218() override;
    virtual void vt_0x220() override;

    void determineNextMove();
    void fn_227_1EA0();
    void fn_227_3DB0();
    void fn_227_3DF0();

private:
    /* 0x220C */ bool field_0x220C;
    /* 0x2210 */ dAcRef_c<dAcObjGirahimuKnife_c> mKnives[5];
};

#endif
