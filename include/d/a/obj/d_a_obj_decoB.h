#ifndef D_A_OBJ_DECOB_H
#define D_A_OBJ_DECOB_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcODecoB_c : public dAcObjBase_c {
public:
    dAcODecoB_c() : mStateMgr(*this) {}
    virtual ~dAcODecoB_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcODecoB_c, Wait);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcODecoB_c);
    /* 0x38C */ u16 padding_0x38C;
    /* 0x38E */ mAng field_0x38E;

    static const f32 lbl_611_rodata_30;
};

#endif
