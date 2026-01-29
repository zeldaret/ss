#ifndef D_A_OBJ_ISLAND_NUSI_H
#define D_A_OBJ_ISLAND_NUSI_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOislandNusi_c : public dAcObjBase_c {
public:
    dAcOislandNusi_c() : mStateMgr(*this) {}
    virtual ~dAcOislandNusi_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOislandNusi_c, Wait);
    STATE_FUNC_DECLARE(dAcOislandNusi_c, NusiFight);

private:
    static const f32 someFloat;

    bool isInLeviasFight() const;

    /* 0x330 */ m3d::smdl_c mMdls[2];
    /* 0x360 */ nw4r::g3d::ResFile mRes;
    /* 0x36C */ dBgW mBgW;
    /* 0x57C */ STATE_MGR_DECLARE(dAcOislandNusi_c);
};

#endif
