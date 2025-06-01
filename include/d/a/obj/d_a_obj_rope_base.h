#ifndef D_A_OBJ_ROPE_BASE_H
#define D_A_OBJ_ROPE_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOropeBase_c : public dAcObjBase_c {
public:
    dAcOropeBase_c() {}
    virtual ~dAcOropeBase_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ mVec3_c mVec;
    /* 0x35C */ bool mBool;
};

#endif
