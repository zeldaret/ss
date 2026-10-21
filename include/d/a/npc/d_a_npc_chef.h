#ifndef D_A_NPC_CHEF_H
#define D_A_NPC_CHEF_H

#include "d/a/npc/d_a_ordinary_npc.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"

class dAcNpcChef_c : public dAcOrdinaryNpc_c {
public:
    dAcNpcChef_c() {}
    virtual ~dAcNpcChef_c() {}

private:
    /* 0x AB4 */ dShadowCircle_c mShadow;
    /* 0x ABC */ dBgS_AcchCir mAcchCir;
    /* 0x B18 */ dBgS_ObjAcch mAcch;
    /* 0x EC8 */ dCcD_Cyl mCyl;
    /* 0x1018 */ m3d::smdl_c mPotMdl; // Assumption
    /* 0x1034 */
};

#endif
