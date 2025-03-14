#ifndef D_A_OBJ_SHRINE_BEF_INSIDE_H
#define D_A_OBJ_SHRINE_BEF_INSIDE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"

class dAcOshrineBefInside_c : public dAcObjBase_c {
public:
    dAcOshrineBefInside_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOshrineBefInside_c() {}
    virtual int actorExecute() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel[2];
    /* 0x36C */ dBgW mCollision;
};

#endif
