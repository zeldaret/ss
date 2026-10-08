#ifndef D_A_B_GIRAHIMU2_H
#define D_A_B_GIRAHIMU2_H

#include "d/a/b/d_a_b_girahimu_base.h"
#include "d/a/d_a_base.h"
#include "s/s_State.hpp"

class dAcObjGirahimuKnife_c;

class dAcGirahimu2_c : public dAcGirahimuBase_c {
public:
    dAcGirahimu2_c() : field_0x221E(false) {}
    virtual ~dAcGirahimu2_c() {}

    virtual int doDelete() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorExecute() override;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu2_c, dAcGirahimuBase_c, Walk);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu2_c, dAcGirahimuBase_c, Catch);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu2_c, dAcGirahimuBase_c, CatchDamage);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcGirahimu2_c, dAcGirahimuBase_c, Counter);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, G_SwordDamage); // Non virtual override?? (still virtual)

    STATE_FUNC_DECLARE(dAcGirahimu2_c, CreateSpinKnife);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, WaitKnifeAttack);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, KnifeAttack);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, KnifePreAttack);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, G_SwordDemo);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, G_SwordWait);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, G_SwordPiyori);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, Death);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, DeathMiniGame);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, UpWarp);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, UpAttack);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, CreateKnife);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, ComeOn);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, ComeOnGuard);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, GuardJustCounter);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, TwoSwordAttack);
    STATE_FUNC_DECLARE(dAcGirahimu2_c, HitAction);

    virtual void vt_0x1DC() override;
    virtual void vt_0x1F4() override;
    virtual void vt_0x1F8() override;
    virtual void vt_0x1FC() override;
    virtual void vt_0x200() override;
    virtual void vt_0x204() override;
    virtual void vt_0x20C() override;
    virtual bool vt_0x210() override;
    virtual void vt_0x214() override;
    virtual bool vt_0x218() override;
    virtual void vt_0x220() override;
    virtual void vt_0x224() override {}
    virtual void vt_0x228() override;
    virtual void vt_0x22C() override;
    virtual void vt_0x230() override;
    virtual void vt_0x234();

    void fn_228_4550(int idxStart, int idxEnd);
    void fn_228_47F0(bool);
    void fn_228_4A30();
    void fn_228_4C20();
    bool fn_228_4CD0(s32 cutDir);
    bool fn_228_5110();
    void fn_228_5400();
    void fn_228_5740();
    bool fn_228_5770(s32 cutDir);
    bool fn_228_5E90();
    bool fn_228_5EC0();
    void fn_228_80A0();
    void fn_228_80E0();
    void fn_228_82C0();

private:
    /* 0x220C */
    /* 0x221E */ bool field_0x221E;
    /* 0x2210 */ dAcRef_c<dAcObjGirahimuKnife_c> mKnives[15];
};

#endif
