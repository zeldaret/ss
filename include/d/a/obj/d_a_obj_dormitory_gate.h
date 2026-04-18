#ifndef D_A_OBJ_DORMITORY_GATE_H
#define D_A_OBJ_DORMITORY_GATE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/stage_render_stuff.h"

class dAcODormitoryGate_c : public dAcObjBase_c {
public:
    enum Type_e {
        TYPE_GATE = 0,
        TYPE_GODDOOR = 1,
        TYPE_SHUTTER = 2,
        TYPE_GATE_F400 = 3,
    };
    dAcODormitoryGate_c() : mStateMgr(*this), mScnCallback(this) {}
    virtual ~dAcODormitoryGate_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcODormitoryGate_c, Wait);
    STATE_FUNC_DECLARE(dAcODormitoryGate_c, End);

private:
    /** Sets the parameter for Door to be open/closed physically */
    void fn_534_C80();

    /** Sets the Door open/closed physically */
    void fn_534_D40();

    bool checkOpenFlag();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdls[2];
    /* 0x36C */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x398 */ dBgW mCollision[2];
    /* 0x7B8 */ STATE_MGR_DECLARE(dAcODormitoryGate_c);
    /* 0x7F4 */ mMtx_c mDoorMtx[2];
    /* 0x854 */ Type_e mSubtype;
    /* 0x858 */ u8 mNumDoors;
    /* 0x85C */ f32 mShutterHeight;
    /* 0x860 */ mAng mGateAngle;
    /* 0x862 */ bool mIsOpen;
    /* 0x863 */ u8 _0x863[0x865 - 0x863];
    /* 0x865 */ u8 mGodDoorSomething;
    /* 0x868 */ dScnCallback_c mScnCallback;
    /* 0x874 */ u16 mStoryflag;
};

#endif
