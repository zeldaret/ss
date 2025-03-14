#ifndef D_A_OBJ_IRON_STAGE_H
#define D_A_OBJ_IRON_STAGE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOironStage_c : public dAcObjBase_c {
public:
    dAcOironStage_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOironStage_c() {}
    virtual int actorExecute() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ dBgW mCollision;
};

#endif
