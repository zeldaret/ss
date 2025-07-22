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

private:
    bool checkStoryFlag() const;

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ u16 mStoryFlag;
};

#endif
