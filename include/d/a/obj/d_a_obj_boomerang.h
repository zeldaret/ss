#ifndef D_A_OBJ_BOOMERANG_H
#define D_A_OBJ_BOOMERANG_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "d/lyt/d_lyt_fader.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_fader.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcBoomerangProc_c : public d3d::UnkProc {
public:
    dAcBoomerangProc_c() : mpMdl(nullptr), mClr(0xFFFFFFFF) {}
    ~dAcBoomerangProc_c() {}

    bool create(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc);
    bool create2(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc) {
        return create(mdl, clr, prioOpa, alloc);
    }

    virtual void drawOpa() override;

private:
    m3d::mdl_c *mpMdl;
    mColor mClr;
};

class dAcBoomerang_c : public dAcObjBase_c {
public:
    dAcBoomerang_c()
        : mStateMgr(*this),
          mEventRelated(*this, nullptr),
          mEff0(this),
          mEff1(this),
          mEff2(this),
          mEff3(this),
          mLytFader(0, dLytFader_c::FADED_OUT) {}
    virtual ~dAcBoomerang_c();

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual void deleteReady() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    virtual void registerInEvent() override;

    STATE_FUNC_DECLARE(dAcBoomerang_c, Wait);
    STATE_FUNC_DECLARE(dAcBoomerang_c, Move);
    STATE_FUNC_DECLARE(dAcBoomerang_c, MoveCancelWait);
    STATE_FUNC_DECLARE(dAcBoomerang_c, ReturnWait);
    STATE_FUNC_DECLARE(dAcBoomerang_c, EventReturnWait);

public: // TYPES
    enum Variant_e {
        NONE,
        BEETLE,
        HOOK_BEETLE,
        QUICK_BEETLE,
        TOUGH_BEETLE,
    };

    struct ChrAnimation_t {
        const char *mName;
        f32 mRate;
    };
    enum ChrAnimation_e {
        RB_SET = 0,
        RB_DEFAULT = 1,
        RB_CUT = 2,
        RB_HOLD = 3,
        RB_HOLD_ED = 4,
        RB_BACK = 5,
        RB_MAX,
    };
    static const ChrAnimation_t sChrAnims[RB_MAX];

    enum ModelType_e {
        MDL_WINGS_BASIC = 0,
        MDL_BODY_BASIC = 1,
        MDL_MOUTH_BASIC = 2,
        MDL_MOUTH_HOOK = 3,
        MDL_MOTTH_ADV = MDL_MOUTH_HOOK, // Just for naming :P
        MDL_BODY_ADV = 4,
        MDL_WINGS_ADV = 5,
    };

    struct FlyChrAnimation_t {
        const char *mName;
    };
    enum FlyAnimation_e {
        RB_FLY = 0,
        RB_FLY_FAST = 1,
        RB_FLY_MAX = 2,
    };

    enum Flags_e {
        /* 0x00000001 */ FLAG_CANCEL_1 = 1 << 0,
        /* 0x00000002 */ FLAG_CANCEL_2 = 1 << 1,
        /* 0x00000003 */ FLAG_CANCEL = FLAG_CANCEL_1 | FLAG_CANCEL_2,

        /* 0x00000004 */ FLAG_RELEASE_OBJECT = 1 << 2,
        /* 0x00000008 */ FLAG_0x8 = 1 << 3,
        /* 0x00000010 */ FLAG_0x10 = 1 << 4,
        /* 0x00000020 */ FLAG_0x20 = 1 << 5,
        /* 0x00000040 */ FLAG_0x40 = 1 << 6,
        /* 0x00000080 */ FLAG_0x80 = 1 << 7,
        /* 0x00000100 */ FLAG_STOP_TIMER_ACTIVE = 1 << 8,
        /* 0x00000200 */ FLAG_DROP_ITEM = 1 << 9,      // related to 0x400
        /* 0x00000400 */ FLAG_REQUEST_0x400 = 1 << 10, // related to 0x200
        /* 0x00000800 */ FLAG_REQUEST_MOVE = 1 << 11,
        /* 0x00001000 */ FLAG_CONTROLLABLE = 1 << 12,
        /* 0x00002000 */ FLAG_0x2000 = 1 << 13,
        /* 0x00004000 */ FLAG_RUMBLE_ACTIVE = 1 << 14,
        /* 0x00008000 */ FLAG_SPEED_UP = 1 << 15,
        /* 0x00010000 */ FLAG_0x10000 = 1 << 16,
        /* 0x00020000 */ FLAG_0x20000 = 1 << 17,
        /* 0x00040000 */ FLAG_0x40000 = 1 << 18,
        /* 0x00080000 */ FLAG_0x80000 = 1 << 19,

        /* 0x0001A117 */ FLAG_COMMON_INIT = FLAG_0x10000 | FLAG_SPEED_UP | FLAG_0x2000 | FLAG_STOP_TIMER_ACTIVE |
                                            FLAG_0x10 | FLAG_RELEASE_OBJECT | FLAG_CANCEL,
    };

public: // INLINES
    bool checkField_0x8CC(u32 mask) const {
        return field_0x8CC & mask;
    }
    void setField_0x8CC(u32 mask) {
        field_0x8CC |= mask;
    }
    void unsetField_0x8CC(u32 mask) {
        field_0x8CC &= ~mask;
    }
    bool isMoving() {
        return mStateMgr.isState(StateID_Move);
    }

    static const s32 getSrcFlags() {
        return 0x209B;
    }

public: // FUNCTIONS
    void areaCallback(mVec3_c *param1, u32 param2);
    void atHitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB);

    /** hides the given shape within the resfile. Ref Brawlcrate's 'polygon#' in the mdl0/Object folder */
    void hideModel(ModelType_e type);

    /** ?? */
    void deleteCheck();

    /** ?? Retrieve Beetle  */
    void retrieve();

    /** Attempts to grab ahold of the object upon collision. Called from atHitCallback if type > Hook beetle */
    bool tryGrabObject(dAcObjBase_c *pObject);

    /** Sets the room id of the beetle. Sets to links room id or the room id of the collision its around */
    void setRoomId();

    /** Updates the position based on where links wrist is  */
    void placeOnArm();

    /**  */
    void fn_80261E10();

    /**  */
    void fn_80262150();

    /**  */
    void calcVelocity();

    /**  */
    void playFlySound();

    /**  */
    void startHitEffect(const mVec3_c &position, const cBgS_PolyInfo &polyInfo);

    /**  */
    bool fn_80262400();

    /** Gets the colliding position and the poly info of where collision happened. Returns a nullptr on no collision */
    cBgS_PolyInfo *getCollidingPolyInfo(mVec3_c &position);

    /**  */
    void fn_80262BE0();

    /**  */
    void fn_80262DC0();

    /** Sets the assuming bonk flag and plays the rumble feedback */
    void startRumble();

    /** Applies the selected animation to the beetle */
    void setChrAnimation(ChrAnimation_e requestedAnimation);

    /** Applies the selected animation to the beetle */
    void setFlyChrAnimation(FlyAnimation_e requestedAnimation);

    /** Sets the amount of time the beetle can fly. Setting a number less than 0 gives it 5s of time */
    void setRemainingFlightTime(s16 time);

    /** Attempts to flash the beetle red at end of flight */
    void executeTimeWarning();

private:
    /* 0x0330 */ nw4r::g3d::ResFile mResFile;
    /* 0x0334 */ m3d::mdl_c mMdl;
    /* 0x0358 */ m3d::anmChrBlend_c mAnmChrBlend;
#define BOOMERANG_ANIM_PINCERS 0
#define BOOMERANG_ANIM_WINGS 1
    /* 0x0380 */ m3d::anmChr_c mAnmChr[2]; /* 0 - Pincers, 1 - Wings*/
    /* 0x03F0 */ dAcBoomerangProc_c mProc;
    /* 0x0410 */ dShadowCircle_c mShadow;
    /* 0x0418 */ dAcRef_c<dAcObjBase_c> mGrabbedActor;
    /* 0x0424 */ u8 _0x424[0x454 - 0x424];
    /* 0x0454 */ dBgS_Acch mAcch;
    /* 0x0804 */ dBgS_AcchCir mAcchCir;
    /* 0x0860 */ ActorEventRelated mEventRelated;
    /* 0x08B0 */ u8 mCurrentAnimation;
    /* 0x08B1 */ u8 field_0x8B1;
    /* 0x08B2 */ u8 field_0x8B2;
    /* 0x08B3 */ u8 mFlashTimer;
    /* 0x08B4 */ u8 field_0x8B4;
    /* 0x08B5 */ u8 mWindNodeID;
    /* 0x08B6 */ u8 mLeftWingNodeID;
    /* 0x08B7 */ u8 mRightWingNodeID;
    /* 0x08B8 */ mAng3_c field_0x8B8;
    /* 0x08BE */ mAng3_c field_0x8BE;
    /* 0x08C4 */ mAng3_c field_0x8C4;
    /* 0x08CA */ u16 mRemainingFlightTime;
    /* 0x08CC */ u32 field_0x8CC;
    /* 0x08D0 */ f32 field_0x8D0;
    /* 0x08D4 */ f32 field_0x8D4;
    /* 0x08D8 */ mVec3_c field_0x8D8;
    /* 0x08E4 */ mVec3_c field_0x8E4;
    /* 0x08F0 */ mVec3_c mBounceVelocity;
    /* 0x08FC */ dCcD_Sph mSph0;
    /* 0x0A4C */ dCcD_Sph mSph1;
    /* 0x0B9C */ dEmitter_c mEff0;
    /* 0x0BD0 */ dEmitter_c mEff1;
    /* 0x0C04 */ dEmitter_c mEff2;
    /* 0x0C38 */ dEmitter_c mEff3;
    /* 0x0C6C */ dLytFader_c mLytFader;
    /* 0x114C */ mFader_c mFader;
    /* 0x1150 */ u8 _0x1150[0x115C - 0x1150];
    /* 0x115C */ STATE_MGR_DECLARE(dAcBoomerang_c);

    static const dCcD_SrcSph sSphSrc;
    static dBgS_BeetleLinChk sLinChk;
};

#endif
