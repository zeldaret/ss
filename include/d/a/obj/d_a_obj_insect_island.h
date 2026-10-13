#ifndef D_A_OBJ_INSECT_ISLAND_H
#define D_A_OBJ_INSECT_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/d_d3d.h"

class dAcOinsectIsland_c : public dAcObjBase_c {
public:
    dAcOinsectIsland_c() {}
    virtual ~dAcOinsectIsland_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

private:
    /* 0x330 */ d3d::UnkWithWater mWtr;
    /* 0x348 */ m3d::smdl_c mMainMdl;
    /* 0x364 */ m3d::smdl_c mWtrMdl0;
    /* 0x380 */ m3d::smdl_c mWtrMdl1;
    /* 0x39C */ m3d::anmTexSrt_c mMainAnm;
    /* 0x3C8 */ m3d::anmTexSrt_c mWtrAnm0;
    /* 0x3F4 */ m3d::anmTexSrt_c mWtrAnm1;
    /* 0x420 */ nw4r::g3d::ResFile mRes;
    /* 0x424 */ dBgW mBgW;
    /* 0x634 */ dBgW mBgWBird;
};

#endif
