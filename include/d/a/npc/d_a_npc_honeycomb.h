#ifndef D_A_NPC_HONEYCOMB_H
#define D_A_NPC_HONEYCOMB_H

#include "common.h"
#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/d_cc.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcNpcHc_c : public dAcEnBase_c {
public:
    dAcNpcHc_c() : mRes(nullptr) {}
    virtual ~dAcNpcHc_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    u8 getBeeAngerTimer() const {
        return mBeeAngerTimer;
    }

    bool isPlayerObstructed() const {
        return mPlayerObstructed;
    }

    const mVec3_c &getActualPosition() const {
        return mActualPosition;
    }

private:
    /* 0x374 */ nw4r::g3d::ResFile mRes;
    /* 0x378 */ m3d::smdl_c mModel;
    /* 0x398 */ dBgS_AcchCir mAcchCir;
    /* 0x3F4 */ dBgS_ObjAcch mObjAcch;
    /* 0x7A4 */ dCcD_Linked_Sph mSph;
    /* 0x904 */ dColliderLinkedList mCollider;
    /* 0x910 */ u8 mNoCollisionTimer;
    /* 0x911 */ bool mIsBlown;
    /* 0x912 */ u8 mFallDownTimer;
    /* 0x913 */ u8 mDeathTimer;
    /* 0x914 */ u8 mGustBellowsTimer;
    /* 0x915 */ u8 mBeeAngerTimer;
    /* 0x916 */ bool mPlayerObstructed;
    /* 0x917 */ u8 mSceneflag;
    /* 0x918 */ s8 field_0x918;
    /* 0x919 */ // padding
    /* 0x91A */ s16 mUnusedTimer1;
    /* 0x91C */ s16 mUnusedTimer2;
    /* 0x91E */ s16 mRandomnessTimer;
    /* 0x920 */ f32 field_0x920;
    /* 0x924 */ f32 field_0x924;
    /* 0x928 */ mVec3_c mActualPosition;
    /* 0x934 */ mVec3_c field_0x934;

    void handleCollider();
};

#endif
