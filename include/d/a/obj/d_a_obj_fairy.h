#ifndef D_A_OBJ_FAIRY_H
#define D_A_OBJ_FAIRY_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dAcOFairy_c : public dAcObjBase_c {
public:
    dAcOFairy_c() : field_0x330(0), mStateMgr(*this) {}
    virtual ~dAcOFairy_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOFairy_c, Wait);
    STATE_FUNC_DECLARE(dAcOFairy_c, Avoid);
    STATE_FUNC_DECLARE(dAcOFairy_c, PlayerAvoid);
    STATE_FUNC_DECLARE(dAcOFairy_c, CureStart);
    STATE_FUNC_DECLARE(dAcOFairy_c, Cure);
    STATE_FUNC_DECLARE(dAcOFairy_c, CureEnd);
    STATE_FUNC_DECLARE(dAcOFairy_c, CatchDemo);

private:
    enum SpawnType_e {
        SPAWN_0,
        SPAWN_1,
        /** The player released the fairy from a bottle or the bug net */
        SPAWN_MANUAL_RELEASE,
        /** The previously bottled fairy saves the player from death */
        SPAWN_AUTO_RELEASE,
    };

    bool shouldAvoidBugNet() const;
    bool isCuring() const;
    bool canTargetWithBugNet() const;
    bool shouldAvoidLink() const;
    bool isMovingAwayFromOrigY() const;

    mVec3_c calcInitialSpawnPosition();
    void setSpawnPosition(const mVec3_c &);

    void calcCurePosition(const f32 &xzOffsetTarget, const f32 &yOffsetTarget);
    bool calcPosYWave();
    void setPosYWaveParams();

    void executeCheckForWindOrBellows();

    f32 randMaxSpeedY() const;
    mAng rndTurnSpeed() const;

    /* 0x330 */ UNKWORD field_0x330; // might be a nw4r::g3d::ResFile
    /* 0x334 */ m3d::mdlAnmChr mModel;
    /* 0x39C */ dShadowCircle_c mShadow;
    /* 0x3A4 */ dBgS_AcchCir mAcchCir;
    /* 0x400 */ dBgS_ObjAcch mObjAcch;
    /* 0x7B0 */ dCcD_Sph mCcSph1;
    /* 0x900 */ dCcD_Sph mCcSph2;
    /* 0xA50 */ STATE_MGR_DECLARE(dAcOFairy_c);
    /* 0xA8C */ dEmitter_c mEffects[2];
    /* 0xAF4 */ mVec3_c mOrigPosition;  ///< The original position of the actor around which it is moving
    /* 0xB00 */ mVec3_c mSpawnPosition; ///< The (slightly randomized) spawn position
    /* 0xB0C */ u8 _0xB0C[0xB18 - 0xB0C];
    /* 0xB18 */ mVec3_c mCurePosition; ///< When curing the player, holds the calculated
                                       ///< position that is applied in actorExecute
    /* 0xB24 */ mMtx_c field_0xB24;
    /* 0xB54 */ mAng mPosYWaveTime;
    /* 0xB56 */ mAng mPosYWaveSpeed;
    /* 0xB58 */ mAng mTurnSpeedY;
    /* 0xB5A */ mAng mCureAngle;        ///< When curing the player, holds the angle on the
                                        ///< circular path around the player
    /* 0xB5C */ mAng mCureAngularSpeed; ///< When curing the player, holds the speed with which
                                        ///< the fairy is circling around the player
    /* 0xB60 */ u32 mPosYWaveAmplitude;
    /* 0xB64 */ f32 mMaxSpeedY;
    /* 0xB68 */ f32 targetSpeedY;
    /* 0xB6C */ f32 mOriginalGndHeight;
    /* 0xB70 */ f32 mCurePosXZOffset;
    /* 0xB74 */ f32 mCurePosXZOffsetTarget;
    /* 0xB78 */ f32 mCurePosYOffset;
    /* 0xB7C */ f32 mAutoReleaseProgress;
    /* 0xB80 */ u8 mSpawnType;
    /* 0xB81 */ u8 field_0xB81;
    /* 0xB82 */ u8 mPreventAvoidTimer;
    /* 0xB83 */ u8 mAvoidTimer;
    /* 0xB84 */ u8 mPreventCatchAfterSpawnTimer;
    /* 0xB85 */ bool mHasSetTurnSpeedY;
    /* 0xB86 */ bool field_0xB86;
    /* 0xB87 */ u8 _0xB87;
    /* 0xB88 */ bool field_0xB88;
    /* 0xB89 */ bool field_0xB89;
};

#endif
