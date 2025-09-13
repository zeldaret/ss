#ifndef D_A_OBJ_ROULETTE_H
#define D_A_OBJ_ROULETTE_H

#include "d/a/d_a_salbage_obj.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_mdl.h"
#include "s/s_State.hpp"

class dAcObjRoulette_c : public dAcSalbageObj_c {
public:
    dAcObjRoulette_c() : dAcSalbageObj_c(&mMdl, SALVAGE_OBJ_ROULETTE) {}
    virtual ~dAcObjRoulette_c() {}

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcObjRoulette_c, dAcSalbageObj_c, DemoThrow);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcObjRoulette_c, dAcSalbageObj_c, Wait);

private:
    /* 0x970 */ m3d::mdl_c mMdl;
    /* 0x994 */ m3d::anmTexSrt_c mAnm;
};

#endif
