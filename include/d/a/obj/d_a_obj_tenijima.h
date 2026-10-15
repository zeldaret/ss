#ifndef D_A_OBJ_TENIJIMA_H
#define D_A_OBJ_TENIJIMA_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOtenijima_c : public dAcObjBase_c {
public:
    dAcOtenijima_c() {}
    virtual ~dAcOtenijima_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    const char *getModelName();
    const char *getDzbName();
    const char *getPlcName();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ u8 mIsOpened;
    /* 0x561 */ u8 mIsOpenType1;
};

#endif
