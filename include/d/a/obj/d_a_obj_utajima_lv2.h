#ifndef D_A_OBJ_UTAJIMA_LV2_H
#define D_A_OBJ_UTAJIMA_LV2_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOutajimaLv2_c : public dAcObjBase_c {
public:
    dAcOutajimaLv2_c() {}
    virtual ~dAcOutajimaLv2_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    static const f32 someFloat;

    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ nw4r::g3d::ResFile mRes;
    /* 0x350 */ dBgW mBgW;
};

#endif
