#ifndef D_A_OBJ_GIRAHIMU_SWORD_LINK_H
#define D_A_OBJ_GIRAHIMU_SWORD_LINK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcObjGirahimuSwordLink_c : public dAcObjBase_c {
public:
    dAcObjGirahimuSwordLink_c() : mStateMgr(*this) {}
    virtual ~dAcObjGirahimuSwordLink_c() {}

    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Hide);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Equip);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, GetSword);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Throw);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, AtThrow);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Stick);
    STATE_FUNC_DECLARE(dAcObjGirahimuSwordLink_c, Reflect);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObjGirahimuSwordLink_c);
};

#endif
