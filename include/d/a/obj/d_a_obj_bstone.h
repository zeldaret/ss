#ifndef D_A_OBJ_BSTONE_H
#define D_A_OBJ_BSTONE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcObstonec : public dAcObjBase_c {
public:
    dAcObstonec() {}
    virtual ~dAcObstonec() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBg;
    /* 0x560 */ f32 mScaleMag;
    /* 0x564 */ u8 mVariant;
};

#endif
