#ifndef D_A_OBJ_MOLE_COVER_H
#define D_A_OBJ_MOLE_COVER_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOmoleCover_c : public dAcObjBase_c {
public:
    dAcOmoleCover_c() {}
    virtual ~dAcOmoleCover_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
};

#endif
