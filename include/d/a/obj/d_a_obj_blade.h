#ifndef D_A_OBJ_BLADE_H
#define D_A_OBJ_BLADE_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOblade_c : public dAcObjBase_c {
public:
    dAcOblade_c() {}
    virtual ~dAcOblade_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::mdlAnmChr mMdl;
};

#endif
