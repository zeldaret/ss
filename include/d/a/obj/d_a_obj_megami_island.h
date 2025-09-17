#ifndef D_A_OBJ_MEGAMI_ISLAND_H
#define D_A_OBJ_MEGAMI_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOmegamiIsland_c : public dAcObjBase_c {
public:
    dAcOmegamiIsland_c() {}
    virtual ~dAcOmegamiIsland_c() {}

    bool createHeap() override;
    int create() override;
    int doDelete() override;
    int actorExecute() override;
    int draw() override;

private:
    bool hasGoddessStatueFallen();
    inline u8 getVariant() const {
        return mParams & 3;
    }

    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdls[2];
    /* 0x36C */ dBgW mBgW;
    /* 0x57C */ m3d::anmTexSrt_c mAnm;
    /* 0x5A8 */ u8 mVariant;
};

#endif
