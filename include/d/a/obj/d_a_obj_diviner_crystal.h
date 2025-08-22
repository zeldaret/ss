#ifndef D_A_OBJ_DIVINER_CRYSTAL_H
#define D_A_OBJ_DIVINER_CRYSTAL_H

#include "d/a/d_a_salbage_obj.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcODivinerCrystal_c : public dAcSalbageObj_c {
public:
    dAcODivinerCrystal_c() : dAcSalbageObj_c(&mMdl1, SALVAGE_OBJ_DIVINER_CRYSTAL) {}
    virtual ~dAcODivinerCrystal_c() {}

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcODivinerCrystal_c, dAcSalbageObj_c, DemoThrow);
    STATE_FUNC_DECLARE(dAcODivinerCrystal_c, Stay);

private:
    /* 0x970 */ nw4r::g3d::ResFile mResMaybe;
    /* 0x974 */ m3d::smdl_c mMdl1;
    /* 0x990 */ m3d::smdl_c mMdl2;
    /* 0x9AC */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x9D8 */ m3d::anmMatClr_c mAnmMatClr;
};

#endif
