#ifndef D_A_OBJ_SHRINE_AFTER_H
#define D_A_OBJ_SHRINE_AFTER_H

#include "d/a/obj/d_a_obj_base.h"

class dAcOshrineAfter_c : public dAcObjBase_c {
public:
    dAcOshrineAfter_c() {}
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual ~dAcOshrineAfter_c() {}
    virtual int actorExecute() override;

    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mModel;

private:
};

#endif
