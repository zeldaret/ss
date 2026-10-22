#ifndef D_A_NPC_VOLCANO_F2_SALBO_H
#define D_A_NPC_VOLCANO_F2_SALBO_H

#include "d/a/npc/d_a_ordinary_npc.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dAcNpcSlb2_c : public dAcOrdinaryNpc_c {
public:
    dAcNpcSlb2_c() : dAcOrdinaryNpc_c(&mCyl, &mAcch) {}
    virtual ~dAcNpcSlb2_c() {}

    void fn_61_58C0(int, int);
    void fn_61_56C0(const mVec3_c, const mVec3_c *, const mAng *);
    bool fn_61_6A10();
    void fn_61_68D0(int);

private:
    // /* Uses Virtual states*/ STATE_MGR_DECLARE(dAcNpcSlb2_c);

    /* 0x1270 */ dBgS_AcchCir mAcchCir;
    /* 0x12CC */ dBgS_ObjAcch mAcch;
    /* 0x167C */ dCcD_Cyl mCyl;
};

#endif
