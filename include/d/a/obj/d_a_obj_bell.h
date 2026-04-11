#ifndef D_A_OBJ_BELL_H
#define D_A_OBJ_BELL_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/dowsing_target.h"

class dAcObell_c : public dAcObjBase_c {
public:
    dAcObell_c() : mStateMgr(*this), mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dAcObell_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcObell_c, Wait);
    STATE_FUNC_DECLARE(dAcObell_c, WaitRupee);
    STATE_FUNC_DECLARE(dAcObell_c, After);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ dCcD_Sph mSph;
    /* 0x6B0 */ STATE_MGR_DECLARE(dAcObell_c);
    /* 0x6EC */ DowsingTarget mDowsingTarget;
    /* 0x70C */ mVec3_c mRupeeDir;
    /* 0x718 */ mVec3_c field_0x718;
    /* 0x### */ // More
};

#endif
