#ifndef D_A_OBJ_DESERT_DEBRIS_H
#define D_A_OBJ_DESERT_DEBRIS_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOdesertDebris_c : public dAcObjBase_c {
public:
    dAcOdesertDebris_c() {}
    virtual ~dAcOdesertDebris_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool checkStoryFlag();

private:
    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dBgW mBgW;
    u16 mStoryFlag;
};

#endif
