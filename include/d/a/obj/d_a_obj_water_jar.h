#ifndef D_A_OBJ_WATER_JAR_H
#define D_A_OBJ_WATER_JAR_H

#include "common.h"
#include "d/a/d_a_salbage_obj.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcOWaterJar_c : public dAcSalbageObj_c {
public:
    dAcOWaterJar_c() : mEvent(*this, nullptr), dAcSalbageObj_c(&mMdl1, SALVAGE_OBJ_WATER_JAR) {}
    virtual ~dAcOWaterJar_c() {}

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOWaterJar_c, dAcSalbageObj_c, DemoThrow);

private:
    /* 0x970 */ ActorEventRelated mEvent;
    /* 0x9C0 */ nw4r::g3d::ResFile mResMaybe;
    /* 0x9C4 */ m3d::smdl_c mMdl1;
    /* 0x9E0 */ m3d::smdl_c mMdl2;
    /* 0x9FC */ m3d::anmChr_c mAnm;
    /* 0xA34 */ dEmitter_c mEffect;
    /* 0xA68 */ u8 _0xA68[0xA6C - 0xA68];
    /* 0xA6C */ dCcD_Cyl mCcCyl;
};

#endif
