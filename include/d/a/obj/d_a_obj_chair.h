#ifndef D_A_OBJ_CHAIR_H
#define D_A_OBJ_CHAIR_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOChair_c : public dAcObjBase_c {
public:
    enum ChairType {
        CHAIR_A, // Standard Stool
        CHAIR_B, // Standard Chair
        CHAIR_C, // Seat like in Headmaster Room
        CHAIR_D, // Couch
        CHAIR_E, // Bench
        CHAIR_F, // Faron - Stump
        CHAIR_G, // Eldin - In Huts
        CHAIR_H, // Lanayru - 'Past'
        CHAIR_I, // In Front of ET
    };

    dAcOChair_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOChair_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOChair_c, Wait);

    ChairType getChairType(u8 &param);

    void updateChairPos();

    bool isBench() const {
        return (mChairType == CHAIR_D || mChairType == CHAIR_E || mChairType == CHAIR_I);
    }
    bool isChairTypeIdk0() const {
        return (
            mChairType == CHAIR_A || mChairType == CHAIR_F || mChairType == CHAIR_G || mChairType == CHAIR_H ||
            mChairType == CHAIR_I
        );
    }
    bool isChairTypeIdk1() const {
        return mChairType == CHAIR_F || mChairType == CHAIR_H;
    }

    // Might be an inline from dAcBase_c?
    mAng getRelativeYRotationToPlayer() {
        mAng a = getXZAngleToPlayer();
        return a - mRotation.y;
    }

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShadow;

    /* 0x358 */ dCcD_Cyl mCyl;
    /* 0x4A8 */ dBgW mBgW;
    /* 0x6B8 */ dBgS_ObjAcch mObjAcch;
    /* 0xA68 */ dBgS_AcchCir mAcchCir;

    /* 0xAC4 */ STATE_MGR_DECLARE(dAcOChair_c);
    /* 0xB00 */ mVec3_c mChairPos;
    /* 0xB0C */ u8 _B0C[0xB14 - 0xB0C];
    /* 0xB14 */ u32 mSeatNodeID;
    /* 0xB18 */ mAng mChairRot;
    /* 0xB1A */ u8 field_0xB1A;
    /* 0xB1B */ u8 field_0xB1B;
    /* 0xB1C */ u8 field_0xB1C;
    /* 0xB1D */ u8 mHealTimer;
    /* 0xB1E */ u8 mChairType;
    /* 0xB1E */ u8 mSceneflag;
};

#endif
