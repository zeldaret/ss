#ifndef D_A_OBJ_FLAG_H
#define D_A_OBJ_FLAG_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcODesertAgo_c;
class dAcOFlag_c : public dAcObjBase_c {
public:
    class callback_c : public nw4r::g3d::IScnObjCallback {
    public:
        callback_c(dAcObjBase_c *arg) : mpActor(arg) {}
        /* 0x04 */ dAcObjBase_c *mpActor;
    };
    dAcOFlag_c() : mResFile(nullptr), mStateMgr(*this), mCallback(this) {}
    virtual ~dAcOFlag_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOFlag_c, Wait);

    void setCollider();
    void createPlacement();

    bool isSail();
    bool isPurpleFlag();
    const char *getArcName();
    const char *getMdlName();
    const char *getAnmTexSrtName();

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x37C */ dCcD_Cps mCps;
    /* 0x4EC */ STATE_MGR_DECLARE(dAcOFlag_c);
    /* 0x528 */ callback_c mCallback;
    /* 0x530 */ s32 field_0x530;
    /* 0x534 */ s32 mNumJoints;
    /* 0x538 */ mVec3_c *mpJoints;
    /* 0x53C */ mVec3_c field_0x53C;
    /* 0x548 */ s32 field_0x548;
    /* 0x54C */ dAcRef_c<dAcODesertAgo_c> mDesertAgoRef;
    /* 0x558 */ s32 field_0x558;
    /* 0x55C */ s32 field_0x55C;
    /* 0x560 */ u8 field_0x560;
    /* 0x561 */ u8 field_0x561;
    /* 0x564 */ s32 field_0x564;
    /* 0x568 */ f32 field_0x568;
    /* 0x56C */ mVec3_c *mpJoints1; // Size 15
    /* 0x570 */ mVec3_c *mpJoints2; // Size 15
    /* 0x574 */ mVec3_c *mpJoints3; // Size 15
};

#endif
