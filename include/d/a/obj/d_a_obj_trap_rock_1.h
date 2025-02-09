#ifndef D_A_OBJ_TRAP_ROCK_1_H
#define D_A_OBJ_TRAP_ROCK_1_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOtrapRock1_c : public dAcObjBase_c {
public:
    dAcOtrapRock1_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOtrapRock1_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapWait);
    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapAction);
    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapReturn);

private:
    /* 0x300 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgW;
    /* 0x560 */ STATE_MGR_DECLARE(dAcOtrapRock1_c);
    /* 0x59C */ u8 mActivationSceneFlag;
    /* 0x59D */ u8 mReturnSceneFlag;
    /* 0x59E */ s16 field_0x59E;
    /* 0x5A0 */ s16 field_0x5A0;
    /* 0x5A2 */ s16 field_0x5A2;
    /* 0x5A4 */ u8 mFrameCounter;
    /* 0x5A5 */ s8 field_0x5A5;
};

#endif
