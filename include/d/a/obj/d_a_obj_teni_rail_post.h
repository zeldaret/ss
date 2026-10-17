#ifndef D_A_OBJ_TENI_RAIL_POST_H
#define D_A_OBJ_TENI_RAIL_POST_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"

class dAcOTeniRailPost : public dAcObjBase_c {
public:
    dAcOTeniRailPost();
    virtual ~dAcOTeniRailPost();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual bool createHeap() override;

private:
    /* 0x330 */ m3d::smdl_c mMdlT;
    /* 0x34C */ m3d::smdl_c mMdlN;
    /* 0x368 */ dBgW mBgW;
    /* 0x578 */ f32 mDist;
    /* 0x57C */ bool mIsInPast;
};

#endif
