#ifndef D_A_OBJ_SHRINE_BEFORE_H
#define D_A_OBJ_SHRINE_BEFORE_H

#include "d/a/obj/d_a_obj_base.h"

class dAcOshrineBefore_c : public dAcObjBase_c {
public:
    dAcOshrineBefore_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOshrineBefore_c() {}
    virtual int actorExecute() override;

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;

private:
};

#endif
