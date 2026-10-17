#ifndef D_A_OBJ_KIBAKO_H
#define D_A_OBJ_KIBAKO_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_area.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_path.h"

class dAcOsw_c;
class dAcOScatterSand;

class dAcOkibako_c : public dAcObjBase_c {
public:
    dAcOkibako_c() : mStateMgr(*this), mEventRelated(*this, nullptr) {}
    virtual ~dAcOkibako_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    STATE_FUNC_DECLARE(dAcOkibako_c, Wait);
    STATE_FUNC_DECLARE(dAcOkibako_c, Fire);
    STATE_FUNC_DECLARE(dAcOkibako_c, PushPull);
    STATE_FUNC_DECLARE(dAcOkibako_c, RopeGrab);
    STATE_FUNC_DECLARE(dAcOkibako_c, DeleteEvent);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcOkibako_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOkibako_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcOkibako_c);

    enum Type_e {
        TYPE_NORMAL,
        TYPE_HANG,
        TYPE_MAX
    };

    dAcObjBase_c *PushPull_CallBack(dAcObjBase_c *, dBgW_Base::PushPullLabel);

    dAcOsw_c *getSwitch(const mVec3_c &pos);

    void checkBreakHeight();
    bool checkHitBomb();
    void updateGravity(bool isFree);
    void breakBox();
    void updatePathGoalFlag();
    void calcFall();

    bool checkCanTryMove(s16 dir, bool set);
    void checkPushPull();
    bool isSwitchInArea(const mVec3_c &pos);
    bool checkNotHittingNewSwitch();
    void crrPos();

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ dShadowCircle_c mShadow;
    /* 0x354 */ dBgW mBgW;
    /* 0x564 */ dBgS_AcchCir mAcchCir;
    /* 0x5C0 */ dBgS_ObjAcch mAcch;
    /* 0x970 */ dCcD_Cyl mCyl;
    /* 0xAC0 */ dCcD_Unk mCcBox;
    /* 0xCC8 */ STATE_MGR_DECLARE(dAcOkibako_c);
    /* 0xD04 */ dAcRef_c<dAcOsw_c> mSw;
    /* 0xD10 */ dAcRef_c<dAcOScatterSand> mScatterSand;
    /* 0xD1C */ dArea_c mArea;
    /* 0xD4C */ ActorEventRelated mEventRelated;
    /* 0xD9C */ ActorOnRail3 mPath;
    /* 0xDC0 */ mVec3_c mSpawnPosition;
    /* 0xDCC */ mVec3_c mPreMovePosition;
    /* 0xDD8 */ f32 mGroundHeight;
    /* 0xDDC */ f32 field_0xDDC; // ??
    /* 0xDE0 */ f32 mFallDistance;
    /* 0xDE4 */ u16 mFireTimer;
    /* 0xDE6 */ u16 mMoveCooldown;
    /* 0xDE8 */ u16 mMoveTimer;
    /* 0xDEA */ u16 field_0xDEA;
    /* 0xDEC */ s16 mGravityCheckTimer;
    /* 0xDEE */ u8 mType;
    /* 0xDEF */ u8 mRestrictSceneflag;
    /* 0xDF0 */ u8 mDestroySceneflag;
    /* 0xDF1 */ u8 mEventID;
    /* 0xDF2 */ u8 mPathID;
    /* 0xDF3 */ u8 mRestrictType;
    /* 0xDF4 */ dEmitter_c mEmitters[4];
    /* 0xEC4 */ bool mbPlayerIsPushPull;
    /* 0xEC5 */ bool mbExecuteThisFrame;
    /* 0xEC6 */ bool mbHitByBomb;
    /* 0xEC7 */ bool mbOnlyChangeHeight;
    /* 0xEC8 */ bool mbOnRope;
    /* 0xEC9 */ bool mbCoOn;
    /* 0xECA */ bool mbOnPath;
    /* 0xECB */ bool mbAtLimit;
    /* 0xECC */ bool mbDrawShadow;
};

#endif
