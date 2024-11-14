#ifndef D_A_OBJ_UNDERGROUND_SWITCH_H
#define D_A_OBJ_UNDERGROUND_SWITCH_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "nw4r/g3d/g3d_resmdl.h"
#include "s/s_State.hpp"

class dAcOUgSwitch_c : public dAcObjBase_c {
public:
    dAcOUgSwitch_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOUgSwitch_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOUgSwitch_c, Off);
    STATE_FUNC_DECLARE(dAcOUgSwitch_c, Active);
    STATE_FUNC_DECLARE(dAcOUgSwitch_c, On);

private:
    void setVariant(int variant);
    static const f32 sActivationRadius;
    static const u8 sEventTimer;

    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOUgSwitch_c);
    /* 0x38C */ m3d::anmMatClr_c mAnmClr;
    /* 0x3B8 */ int mVariant;
    /* 0x3BC */ u8 mActiveSceneFlag;
    /* 0x3BD */ u8 field_0x3BD;
    /* 0x3BE */ u8 mOnEventTimer;
};

#endif
