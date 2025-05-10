#ifndef JPAEMITTER_H
#define JPAEMITTER_H

#include "common.h"
#include "rvl/GX.h"
#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAList.h"
#include "JSystem/JParticle/JPARandom.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JSupport/JSUList.h"
#include "egg/math/eggVector.h"
#include "m/m_angle.h"

class JPAResourceManager;
class JPABaseEmitter;
class JPAEmitterManager;
class JPAParticleCallBack;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAEmitterWorkData {
    struct JPAVolumeCalcData {
        /* 0x00 */ EGG::Vector3f mVolumePos;
        /* 0x0C */ EGG::Vector3f mVelOmni;
        /* 0x18 */ EGG::Vector3f mVelAxis;
    };

    JPAEmitterWorkData() : mRndm(0) {}

    /* 0x00 */ JPABaseEmitter* mpEmtr;
    /* 0x04 */ JPAResource* mpRes;
    /* 0x08 */ JPAResourceManager* mpResMgr;
    /* 0x0C */ JPARandom mRndm;
    /* 0x10 */ JPAVolumeCalcData mVolumeCalcData;
    /* 0x34 */ f32 mVolumeSize;
    /* 0x38 */ f32 mVolumeMinRad;
    /* 0x3C */ f32 mVolumeSweep;
    /* 0x40 */ s32 mEmitCount;
    /* 0x44 */ s32 mVolumeEmitIdx;
    /* 0x48 */ Mtx mDirectionMtx;
    /* 0x78 */ Mtx mRotationMtx;
    /* 0xA8 */ Mtx mGlobalRot;
    /* 0xD8 */ Mtx mGlobalSR;
    /* 0x108 */ EGG::Vector3f mEmitterPos;
    /* 0x114 */ EGG::Vector3f mGlobalScl;
    /* 0x120 */ EGG::Vector3f mGlobalEmtrDir;
    /* 0x12C */ EGG::Vector3f mPublicScale;
    /* 0x138 */ EGG::Vector3f mGlobalPos;
    /* 0x144 */ EGG::Vector2f mGlobalPtclScl;
    /* 0x14C */ EGG::Vector2f mPivot;
    /* 0x154 */ Mtx mYBBCamMtx;
    /* 0x184 */ Mtx mPosCamMtx;
    /* 0x1B4 */ Mtx mPrjMtx;
    /* 0x1E4 */ JPAList<JPABaseParticle>* mpAlivePtcl;
    /* 0x1E8 */ JPANode<JPABaseParticle>* mpCurNode;
    /* 0x1EC */ s32 mVolumeAngleNum;
    /* 0x1F0 */ s32 mVolumeAngleMax;
    /* 0x1F4 */ s32 mVolumeX;
    /* 0x1F8 */ s32 mDivNumber;
    /* 0x1FC */ f32 mScaleAnm;
    /* 0x200 */ u32 mDirType;
    /* 0x204 */ u32 mRotType;
    /* 0x208 */ u32 mPlaneType;
    /* 0x20C */ u32 mDLType;
    /* 0x210 */ u32 mPrjType;
    /* 0x214 */ s16 mClrKeyFrame;
    /* 0x216 */ u8 mDrawCount;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAEmitterCallBack {
public:
    virtual ~JPAEmitterCallBack() = 0;
    virtual void execute(JPABaseEmitter*) {}
    virtual void executeAfter(JPABaseEmitter*) {}
    virtual void draw(JPABaseEmitter*) {}
    virtual void drawAfter(JPABaseEmitter*) {}
        
    //~JPAEmitterCallBack();
};

enum {
    JPAEmtrStts_StopEmit = 0x01,
    JPAEmtrStts_StopCalc = 0x02,
    JPAEmtrStts_StopDraw = 0x04,
    JPAEmtrStts_EnableDeleteEmitter = 0x08,
    JPAEmtrStts_FirstEmit = 0x10,
    JPAEmtrStts_RateStepEmit = 0x20,
    JPAEmtrStts_Immortal = 0x40,
    JPAEmtrStts_Delete = 0x100,
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPABaseEmitter {
public:
    ~JPABaseEmitter() {}
    JPABaseEmitter() : mLink(this), mRndm(0) {}
    void init(JPAEmitterManager*, JPAResource*);
    bool processTillStartFrame();
    bool processTermination();
    void calcEmitterGlobalPosition(EGG::Vector3f*) const;
    void deleteAllParticle();
    JPABaseParticle* createChild(JPABaseParticle*);
    JPABaseParticle* createParticle();
    u32 getCurrentCreateNumber() const;
    u8 getDrawCount() const;
    bool loadTexture(u8, GXTexMapID);

    void initStatus(u32 status) { mStatus = status; }
    void setStatus(u32 status) { mStatus |= status; }
    void clearStatus(u32 status) { mStatus &= ~status; }
    u32 checkStatus(u32 status) const { return (mStatus & status); }
    bool checkFlag(u32 flag) const { return !!(mpRes->getDyn()->getFlag() & flag); }
    u32 getDynResUserWork() const { return mpRes->getDyn()->getResUserWork(); }
    u8 getResourceManagerID() const { return mResMgrID; }
    u8 getGroupID() const { return mGroupID; }
    u8 getDrawTimes() const { return mDrawTimes; }
    void setRate(f32 rate) { mRate = rate; }
    void setDirectionalSpeed(f32 i_speed) { mDirSpeed = i_speed; }
    void setRandomDirectionSpeed(f32 i_speed) { mRndmDirSpeed = i_speed; }
    void setEmitterCallBackPtr(JPAEmitterCallBack* ptr) { mpEmtrCallBack = ptr; }
    void setGlobalRTMatrix(const Mtx m) { JPASetRMtxTVecfromMtx(m, mGlobalRot, &mGlobalTrs); }
    void setGlobalSRTMatrix(const Mtx m) { 
        JPASetRMtxSTVecfromMtx(m, mGlobalRot, &mGlobalScl, &mGlobalTrs);

        // "set is used in TP debug"
        mGlobalPScl.set(mGlobalScl.x, mGlobalScl.y);
    }
    void setGlobalTranslation(f32 x, f32 y, f32 z) { mGlobalTrs.set(x, y, z); }
    void setGlobalTranslation(const EGG::Vector3f& trs) { mGlobalTrs.set(trs); }
    void getLocalTranslation(EGG::Vector3f& vec) { vec.set(mLocalTrs); }
    void setGlobalRotation(const mAng3_c& rot) {
        JPAGetXYZRotateMtx(rot.x, rot.y, rot.z, mGlobalRot); 
    }
    void getGlobalTranslation(EGG::Vector3f* out) const { out->set(mGlobalTrs); }
    void setGlobalDynamicsScale(const EGG::Vector3f& i_scale) { mGlobalScl.set(i_scale); }
    void setGlobalAlpha(u8 alpha) { mGlobalPrmClr.a = alpha; }
    u8 getGlobalAlpha() { return mGlobalPrmClr.a; }
    void getGlobalPrmColor(GXColor& color) { color = mGlobalPrmClr; }
    void setGlobalPrmColor(u8 r, u8 g, u8 b) { mGlobalPrmClr.r = r; mGlobalPrmClr.g = g; mGlobalPrmClr.b = b; }
    void setGlobalEnvColor(u8 r, u8 g, u8 b) { mGlobalEnvClr.r = r; mGlobalEnvClr.g = g; mGlobalEnvClr.b = b; }
    void setVolumeSize(u16 size) { mVolumeSize = size; }
    void setLifeTime(s16 lifetime) { mLifeTime = lifetime; }
    void setAwayFromCenterSpeed(f32 i_speed) { mAwayFromCenterSpeed = i_speed; }
    void setAwayFromAxisSpeed(f32 i_speed) { mAwayFromAxisSpeed = i_speed; }
    void setSpread(f32 i_spread) { mSpread = i_spread; }
    void setLocalTranslation(const EGG::Vector3f& i_trans) { mLocalTrs.set(i_trans); }
    void setLocalRotation(const mAng3_c& i_rot) { mLocalRot.set(i_rot.x * 0.005493248f, i_rot.y * 0.005493248f, i_rot.z * 0.005493248f); }
    void setRateStep(u8 i_step) { mRateStep = i_step; }

    void setGlobalParticleHeightScale(f32 height) {
        mGlobalPScl.y = height;
    }
    void setGlobalParticleScale(const EGG::Vector3f& scale) {
        mGlobalPScl.set(scale.x, scale.y);
    }
    void setGlobalParticleScale(f32 scaleX, f32 scaleY) {
        mGlobalPScl.set(scaleX, scaleY);
    }
    void getGlobalParticleScale(EGG::Vector3f& scale) {
        scale.set(mGlobalPScl.x, mGlobalPScl.y, 1.0f);
    }
    void setGlobalScale(const EGG::Vector3f& scale) {
        mGlobalScl.set(scale);
        mGlobalPScl.set(scale.x ,scale.y);
    }
    void setGlobalSRTMatrix(const MtxP matrix) {
        JPASetRMtxSTVecfromMtx(matrix, mGlobalRot, &mGlobalScl, &mGlobalTrs);
        mGlobalPScl.set(mGlobalScl.x, mGlobalScl.y);
    }
    void setDirection(const EGG::Vector3f& direction) {
        mLocalDir.set(direction);
    }

    void setLocalScale(const EGG::Vector3f& scale) {
        mLocalScl.set(scale);
    }

    f32 getGlobalParticleScaleX() const {
        return mGlobalPScl.x;
    }

    f32 getGlobalParticleScaleY() const {
        return mGlobalPScl.y;
    }

    f32 get_r_f() { return mRndm.get_rndm_f(); }
    f32 get_r_zp() { return mRndm.get_rndm_zp(); }
    f32 get_r_zh() { return mRndm.get_rndm_zh(); }
    s16 get_r_ss() { return mRndm.get_rndm_ss(); }

    void stopCreateParticle() { setStatus(JPAEmtrStts_StopEmit); }
    void playCreateParticle() { clearStatus(JPAEmtrStts_StopEmit); }
    void becomeImmortalEmitter() { setStatus(JPAEmtrStts_Immortal); }
    void becomeContinuousParticle() { mMaxFrame = 0; }
    void becomeDeleteEmitter() { setStatus(JPAEmtrStts_Delete); }
    void becomeInvalidEmitter() {
        stopCreateParticle();
        mMaxFrame = 1;
    }

    void quitImmortalEmitter() { clearStatus(JPAEmtrStts_Immortal); }
    void stopCalcEmitter() { setStatus(JPAEmtrStts_StopCalc); }
    void playCalcEmitter() { clearStatus(JPAEmtrStts_StopCalc); }
    void stopDrawParticle() { setStatus(JPAEmtrStts_StopDraw); }
    void playDrawParticle() { clearStatus(JPAEmtrStts_StopDraw); }

    u32 getUserWork() { return mpUserWork; }
    void setUserWork(u32 userWork) { mpUserWork = userWork; }
    u32 getParticleNumber() {
        return mAlivePtclBase.getNum() + mAlivePtclChld.getNum();
    }
    bool isEnableDeleteEmitter() {
        return checkStatus(JPAEmtrStts_EnableDeleteEmitter) && getParticleNumber() == 0;
    }
    void setDrawTimes(u8 drawTimes) { mDrawTimes = drawTimes; }
    void setParticleCallBackPtr(JPAParticleCallBack* cb) { mpPtclCallBack = cb; }
    JPAParticleCallBack* getParticleCallBackPtr() { return mpPtclCallBack; }
    JPAEmitterCallBack* getEmitterCallBackPtr() const { return mpEmtrCallBack; }
    u32 getAge() const { return mTick; }

public:
    /* 0x00 */ EGG::Vector3f mLocalScl;
    /* 0x0C */ EGG::Vector3f mLocalTrs;
    /* 0x18 */ EGG::Vector3f mLocalDir;
    /* 0x24 */ s32 mMaxFrame;
    /* 0x28 */ f32 mRate;
    /* 0x2C */ f32 mVolumeSweep;
    /* 0x30 */ f32 mVolumeMinRad;
    /* 0x34 */ f32 mAwayFromCenterSpeed;
    /* 0x38 */ f32 mAwayFromAxisSpeed;
    /* 0x3C */ f32 mDirSpeed;
    /* 0x40 */ f32 mSpread;
    /* 0x44 */ f32 mRndmDirSpeed;
    /* 0x48 */ f32 mAirResist;
    /* 0x4C */ mAng3_c mLocalRot;
    /* 0x52 */ s16 mLifeTime;
    /* 0x54 */ u16 mVolumeSize;
    /* 0x56 */ u8 mRateStep;
    /* 0x58 */ JSULink<JPABaseEmitter> mLink;
    /* 0x68 */ Mtx mGlobalRot;
    /* 0x98 */ EGG::Vector3f mGlobalScl;
    /* 0xA4 */ EGG::Vector3f mGlobalTrs;
    /* 0xB0 */ EGG::Vector2f mGlobalPScl;
    /* 0xB8 */ GXColor mGlobalPrmClr;
    /* 0xBC */ GXColor mGlobalEnvClr;
    /* 0xC0 */ s32 mpUserWork;
    /* 0xC4 */ JPARandom mRndm;
    /* 0xC8 */ JPAList<JPABaseParticle> mAlivePtclBase;
    /* 0xD4 */ JPAList<JPABaseParticle> mAlivePtclChld;
    /* 0xE0 */ JPAList<JPABaseParticle>* mpPtclPool;
    /* 0xE4 */ JPAEmitterManager* mpEmtrMgr;
    /* 0xE8 */ JPAResource* mpRes;
    /* 0xEC */ JPAEmitterCallBack* mpEmtrCallBack;
    /* 0xF0 */ JPAParticleCallBack* mpPtclCallBack;
    /* 0xF4 */ u32 mStatus;
    /* 0xF8 */ f32 mEmitCount;
    /* 0xFC */ f32 mScaleOut;
    /* 0x100 */ u32 mTick;
    /* 0x104 */ s16 mWaitTime;
    /* 0x106 */ s16 mRateStepTimer;
    /* 0x108 */ GXColor mPrmClr;
    /* 0x10C */ GXColor mEnvClr;
    /* 0x110 */ u8 mDrawTimes;
    /* 0x111 */ u8 mTexAnmIdx;
    /* 0x112 */ u8 mGroupID;
    /* 0x113 */ u8 mResMgrID;
};

enum {
    JPAPtclStts_Invisible = 0x08,
};

#endif /* JPAEMITTER_H */
