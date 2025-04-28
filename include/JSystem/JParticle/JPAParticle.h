#ifndef JPAPARTICLE_H
#define JPAPARTICLE_H

#include "common.h"
#include "rvl/GX.h"
#include "egg/math/eggVector.h"

class JPABaseEmitter;
class JPABaseParticle;
class JPAEmitterCallBack;
class JPAEmitterManager;
class JPAParticleCallBack;
class JPAResourceManager;
struct JPAEmitterWorkData;

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPABaseParticle {
public:
    ~JPABaseParticle() {}
    void init_p(JPAEmitterWorkData*);
    void init_c(JPAEmitterWorkData*, JPABaseParticle*);
    bool calc_p(JPAEmitterWorkData*);
    bool calc_c(JPAEmitterWorkData*);
    bool canCreateChild(JPAEmitterWorkData*);
    f32 getWidth(JPABaseEmitter const*) const;
    f32 getHeight(JPABaseEmitter const*) const;
    int getAge() { return mAge; }
    void setOffsetPosition(const EGG::Vector3f& pos) { mOffsetPosition.set(pos); }
    void setOffsetPosition(f32 x, f32 y, f32 z) { mOffsetPosition.set(x, y, z); }
    void getOffsetPosition(EGG::Vector3f& pos) { pos.set(mOffsetPosition); }
    s16 getRotateAngle() const { return mRotateAngle; }
    void getGlobalPosition(EGG::Vector3f& pos) const { pos.set(mPosition); }
    f32 getParticleScaleX() const { return mParticleScaleX; }
    f32 getParticleScaleY() const { return mParticleScaleY; }
    void setStatus(u32 flag) { mStatus |= flag; }
    u32 checkStatus(u32 flag) { return mStatus & flag; }
    void initStatus(u32 status) { mStatus = status; }
    void setInvisibleParticleFlag() { setStatus(8); }
    void setDeleteParticleFlag() { setStatus(2); }
    void getVelVec(EGG::Vector3f& vec) const { vec.set(mVelocity); }
    void getLocalPosition(EGG::Vector3f& vec) const { vec.set(mLocalPosition); }
    void getBaseAxis(EGG::Vector3f& vec) const { vec.set(mBaseAxis); }

public:
    /* 0x00 */ EGG::Vector3f mPosition;
    /* 0x0C */ EGG::Vector3f mLocalPosition;
    /* 0x18 */ EGG::Vector3f mOffsetPosition;
    /* 0x24 */ EGG::Vector3f mVelocity;
    /* 0x30 */ EGG::Vector3f mVelType1;
    /* 0x3C */ EGG::Vector3f mVelType0;
    /* 0x48 */ EGG::Vector3f mVelType2;
    /* 0x54 */ EGG::Vector3f mBaseAxis;
    /* 0x60 */ f32 mParticleScaleX;
    /* 0x64 */ f32 mParticleScaleY;
    /* 0x68 */ f32 mScaleOut;
    /* 0x6C */ f32 mAlphaWaveRandom;
    /* 0x70 */ f32 mMoment;
    /* 0x74 */ f32 mDrag;
    /* 0x78 */ u32 field_0x78;
    /* 0x7C */ u32 mStatus;
    /* 0x80 */ s16 mAge;
    /* 0x82 */ s16 mLifeTime;
    /* 0x84 */ f32 mTime;
    /* 0x88 */ u16 mRotateAngle;
    /* 0x8A */ s16 mRotateSpeed;
    /* 0x8C */ GXColor mPrmClr;
    /* 0x90 */ GXColor mEnvClr;
    /* 0x94 */ u8 mTexAnmIdx;
    /* 0x95 */ u8 mAnmRandom;
    /* 0x96 */ u8 mPrmColorAlphaAnm;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAParticleCallBack {
public:
    JPAParticleCallBack() {}
    virtual ~JPAParticleCallBack() = 0;
    virtual void execute(JPABaseEmitter*, JPABaseParticle*) {}
    virtual void draw(JPABaseEmitter*, JPABaseParticle*) {}
};

// not sure where this belongs
static inline u32 COLOR_MULTI(u32 a, u32 b) {
    return ((a * (b + 1)) * 0x10000) >> 24;
}

#endif /* JPAPARTICLE_H */
