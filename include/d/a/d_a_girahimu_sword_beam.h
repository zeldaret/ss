#ifndef D_A_GIRAHIMU_SWORD_BEAM_H
#define D_A_GIRAHIMU_SWORD_BEAM_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcGirahimuSwordBeam_c : public dAcObjBase_c {
public:
    dAcGirahimuSwordBeam_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimuSwordBeam_c() {}

    STATE_FUNC_DECLARE(dAcGirahimuSwordBeam_c, BulletMove);
    STATE_FUNC_DECLARE(dAcGirahimuSwordBeam_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimuSwordBeam_c);
};

#endif
