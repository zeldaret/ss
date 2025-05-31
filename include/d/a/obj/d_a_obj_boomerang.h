#ifndef D_A_OBJ_BOOMERANG_H
#define D_A_OBJ_BOOMERANG_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "d/lyt/d_lyt_fader.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_mdl.h"
#include "m/m_color.h"
#include "m/m_fader.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"

class dAcBoomerangProc_c : public d3d::UnkProc {
public:
    dAcBoomerangProc_c() : mColor0(0), mColor1(0xFFFFFFFF) {}
    ~dAcBoomerangProc_c() {}

    bool create(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc);
    bool create2(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc) {
        return create(mdl, clr, prioOpa, alloc);
    }

    virtual void drawOpa() override;

private:
    mColor mColor0;
    mColor mColor1;
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

    // Macros until enum is solidified
#define FLAG_BOOMERANG_CANCEL (0x3)
#define FLAG_BOOMERANG_RUMBLE_ACTIVE (0x4000)

public: // INLINES
    bool checkField_0x8CC(u32 mask) const {
        return field_0x8CC & mask;
    }
    void onField_0x8CC(u32 mask) {
        field_0x8CC |= mask;
    }
    bool isMoving() {
        return mStateMgr.isState(StateID_Move);
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

    /** Sets the assuming bonk flag and plays the rumble feedback */
    void bonk();

    /** Applies the selected animation to the beetle */
    void setChrAnimation(ChrAnimation_e requestedAnimation);

private:
    /* 0x0330 */ nw4r::g3d::ResFile mResFile;
    /* 0x0334 */ m3d::mdl_c mMdl;
    /* 0x0358 */ m3d::anmChrBlend_c mAnmChrBlend;
#define BOOMERANG_ANIM_PINCERS (0)
#define BOOMERANG_ANIM_WINGS (1)
    /* 0x0380 */ m3d::anmChr_c mAnmChr[2]; /* 0 - Pincers, 1 - Wings*/
    /* 0x03F0 */ dAcBoomerangProc_c mProc;
    /* 0x0410 */ dShadowCircle_c mShadow;
    /* 0x0418 */ dAcRef_c<dAcObjBase_c> mGrabbedActor;
    /* 0x0424 */ u8 _0x424[0x454 - 0x424];
    /* 0x0454 */ dBgS_Acch mAcch;
    /* 0x0804 */ dBgS_AcchCir mAcchCir;
    /* 0x0860 */ ActorEventRelated mEventRelated;
    /* 0x08B0 */ u8 mCurrentAnimation;
    /* 0x08B1 */ u8 _0x8B1[0x8B5 - 0x8B1];
    /* 0x08B5 */ u8 mWindNodeID;
    /* 0x08B6 */ u8 mLeftWingNodeID;
    /* 0x08B7 */ u8 mRightWingNodeID;
    /* 0x08B8 */ u8 _0x8B8[0x8CC - 0x8B8];
    /* 0x08B8 */ u32 field_0x8CC;
    /* 0x08D0 */ u8 _0x8D0[0x8D8 - 0x8D0];
    /* 0x08D8 */ mVec3_c field_0x8D8;
    /* 0x08E4 */ u8 _0x8E4[0x8FC - 0x8E4];
    /* 0x08FC */ dCcD_Sph mSph0;
    /* 0x0A4C */ dCcD_Sph mSph1;
    /* 0x0B9C */ EffectsStruct mEff0;
    /* 0x0BD0 */ EffectsStruct mEff1;
    /* 0x0C04 */ EffectsStruct mEff2;
    /* 0x0C38 */ EffectsStruct mEff3;
    /* 0x0C6C */ dLytFader_c mLytFader;
    /* 0x114C */ mFader_c mFader;
    /* 0x1150 */ u8 _0x1150[0x115C - 0x1150];
    /* 0x115C */ STATE_MGR_DECLARE(dAcBoomerang_c);

    static dCcD_SrcSph sSphSrc;
    static dBgS_BeetleLinChk sLinChk;
    static const u32 sSrcAtType;
};

#endif
