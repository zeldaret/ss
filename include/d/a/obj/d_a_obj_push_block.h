#ifndef D_A_OBJ_PUSH_BLOCK_H
#define D_A_OBJ_PUSH_BLOCK_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_path.h"
#include "toBeSorted/past_present.h"

class dAcOsw_c;
class dAcOScatterSand;
class dAcOstageKraken_c;

class dAcOpushBlk_c : public dAcObjBase_c {
public:
    dAcOpushBlk_c() : mStateMgr(*this) {}
    virtual ~dAcOpushBlk_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOpushBlk_c, Wait);
    STATE_FUNC_DECLARE(dAcOpushBlk_c, Move);
    STATE_FUNC_DECLARE(dAcOpushBlk_c, RopeGrab);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOpushBlk_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOpushBlk_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOpushBlk_c);

    enum Type_e {
        TYPE_NORMAL,
        TYPE_HANG,
        TYPE_MAX
    };

    dAcObjBase_c *PushPull_CallBack(dAcObjBase_c *, dBgW_Base::PushPullLabel);
    dAcOsw_c *getSwitch(const mVec3_c &pos);

    void calcFall();

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdlPast;
    /* 0x350 */ m3d::smdl_c mMdlPresent;
    /* 0x36C */ dShadowCircle_c mShadow;
    /* 0x374 */ dBgW mBgW;
    /* 0x584 */ dBgS_AcchCir mAcchCir[2];
    /* 0x63C */ dBgS_ObjAcch mAcch;
    /* 0x9EC */ dCcD_Cyl mCyl;
    /* 0xB3C */ dCcD_Unk mCcBox;
    /* 0xD44 */ STATE_MGR_DECLARE(dAcOpushBlk_c);
    /* 0xD80 */ dAcRef_c<dAcOsw_c> mSw;
    /* 0xD8C */ dAcRef_c<dAcOstageKraken_c> mStageKraken;
    /* 0xD98 */ dAcRef_c<dAcOScatterSand> mScatterSand;

    /* 0xDD4 */ ActorOnRail3 mPath;
    /* 0xDF8 */ dPastPresent_c mTime;
    /* 0xDFC */ dEmitter_c mEmitter[4];
    /* 0xECC */ mVec3_c mSpawnPosition;

    /* 0xEE4 */ mVec3_c field_0xEE4;

    /* 0xEFA */ s16 field_0xEFA;

    /* 0xEFD */ u8 mType;
    /* 0xEFE */ u8 mGoalSceneflag;
    /* 0xEFF */ u8 mRestrictSceneflag;
    /* 0xF00 */ u8 mbGoalSwitch;
    /* 0xF01 */ u8 mTentalusFight;
    /* 0xF02 */ u8 field_0xF02;
    /* 0xF03 */ u8 mbLargerRadius;
    /* 0xF04 */ bool mbPlayerIsPushPull;
    /* 0xF05 */ bool field_0xF05;
    /* 0xF06 */ bool field_0xF06;
    /* 0xF08 */ bool field_0xF08;
    /* 0xF09 */ bool mbOnPath;
    /* 0xF0A */ bool mbOnRope;

    /* 0xF0C */ bool field_0xF0C;

    /* 0xF0F */ bool field_0xF0F;
};

#endif
