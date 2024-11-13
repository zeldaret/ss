#ifndef D_A_OBJ_TUBO_H
#define D_A_OBJ_TUBO_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/npc/d_a_npc_ce_friend.h"
#include "d/a/npc/d_a_npc_ce_lady.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m3d/m_smdl.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/effects_struct.h"

class dAcOtubo_c : public dAcObjBase_c {
public:
    dAcOtubo_c() : mStateMgr(*this, sStateID::null), mField_0x8F0(this) {}
    virtual ~dAcOtubo_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual void registerInEvent() override;

    bool checkCarryType() const;
    void destroy();

    void calcRoll();
    void adjustRoll();
    void attemptDestroy();
    void fn_272_2670();
    mVec3_c getCenter() const;
    void attemptDestroyOnWall(u32 *, const u8 *);
    void fn_272_2E60(const mVec3_c &);
    void fn_272_3020();
    void addPickupTarget();
    void adjustAngle();
    void adjustSpeed();
    bool checkSlope();
    bool fn_272_3660();
    bool checkInvalidGround();
    bool checkOnLava();
    bool checkSubmerged();
    bool fn_272_38A0();
    bool fn_272_38C0();
    bool checkRollHitMaybe();
    void playRollSound();

    bool checkOnLog_0xE4E() const;

    u8 getSubtype() const {
        return params & 0xF; // 0 -> Tubo00, Else -> Tubo01
    }

    int getParams_0x3000() const {
        return params >> 12 & 0x3;
    }
    u8 getParams_0xC000() const {
        return params >> 14 & 0x3;
    }

    u8 getSceneflag() const {
        return params >> 4 & 0xFF;
    }

    bool checkSceneflag() {
        return SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
    }

    STATE_FUNC_DECLARE(dAcOtubo_c, Wait);
    STATE_FUNC_DECLARE(dAcOtubo_c, Grab);
    STATE_FUNC_DECLARE(dAcOtubo_c, Put);
    STATE_FUNC_DECLARE(dAcOtubo_c, Slope);
    STATE_FUNC_DECLARE(dAcOtubo_c, Rebirth);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShdw;
    /* 0x358 */ dBgS_AcchCir mAcchCir;
    /* 0x3B4 */ dBgS_ObjAcch mObjAcch;
    /* 0x764 */ dCcD_Sph mSph;
    /* 0x8B4 */ STATE_MGR_DECLARE(dAcOtubo_c);
    /* 0x8F0 */ dAcObjRef_unk mField_0x8F0;
    /* 0x91C */ EffectsStruct_Ext mEff_0x91C;
    /* 0x964 */ dAcRef_c<dAcNpcCeLady_c> mCeLady;
    /* 0x970 */ dAcRef_c<dAcNpcCeFriend_c> mCeFriend;
    /* 0x97C */ mQuat_c mQuat_0x97C;
    /* 0x98C */ mQuat_c mQuat_0x98C;
    /* 0x99C */ mQuat_c mQuat_0x99C;
    /* 0x9AC */ mVec3_c mField_0x9AC;
    /* 0x9B8 */ mVec3_c mField_0x9B8;
    /* 0x9C4 */ mAng mField_0x9C4;
    /* 0x9C6 */ mAng mField_0x9C6;
    /* 0x9C8 */ mAng mField_0x9C8;
    /* 0x9CA */ mAng mField_0x9CA;
    /* 0x9CC */ mAng mField_0x9CC;
    /* 0x9D0 */ f32 mField_0x9D0;
    /* 0x9D4 */ f32 mField_0x9D4;
    /* 0x9D8 */ f32 mField_0x9D8;
    /* 0x9DC */ f32 mField_0x9DC;
    /* 0x9E0 */ f32 mField_0x9E0;
    /* 0x9E4 */ f32 mField_0x9E4;
    /* 0x9E8 */ u16 mTimer_0x9E8;
    /* 0x9EA */ bool mbMovingForward;
    /* 0x9EB */ bool mbField_0x9EB;
    /* 0x9EC */ bool mbField_0x9EC;
    /* 0x9ED */ bool mbField_0x9ED;
    /* 0x9EE */ bool mbField_0x9EE;
    /* 0x9EF */ bool mbField_0x9EF;
    /* 0x9F0 */ bool mbField_0x9F0;
    /* 0x9F1 */ bool mbField_0x9F1;
    /* 0x9F2 */ bool mbSubmerged;
    /* 0x9F2 */ bool mbField_0x9F3;
    /* 0x9F4 */ u8 mTimer_0x9F4;
    /* 0x9F5 */ u8 mTimer_0x9F5;
    /* 0x9F6 */ u8 mField_0x9F6;
    /* 0x9F7 */ u8 mTimer_0x9F7;
    /* 0x9F8 */ u8 mSceneflag;
    /* 0x9F9 */ u8 mSubtype;
    /* 0x9FC */ int mField_0x9FC;

    static u8 sUnk0;
    static u32 sUnk1;
    static dCcD_SrcSph sSphSrc;
};

#endif
