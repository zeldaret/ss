#ifndef JPAFIELDBLOCK_H
#define JPAFIELDBLOCK_H

#include "common.h"
#include "egg/egg_types.h"
#include "egg/math/eggVector.h"

class JPAEmitterWorkData;
class JPABaseParticle;
class JPAFieldBlock;

class JPAFieldBase {
public:
    void calcAffect(JPAFieldBlock*, JPABaseParticle*);
    f32 calcFadeAffect(JPAFieldBlock*, f32) const;

    virtual ~JPAFieldBase() {}
    virtual void prepare(JPAEmitterWorkData*, JPAFieldBlock*) {}
    virtual void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*) = 0;

    /* 0x04 */ EGG::Vector3f mAccel;
};

class JPAFieldGravity : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldGravity() {}
};

class JPAFieldAir : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldAir() {}
};

class JPAFieldMagnet : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldMagnet() {}

    /* 0x10 */ EGG::Vector3f mDir;
};

class JPAFieldNewton : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldNewton() {}

    /* 0x10 */ EGG::Vector3f mDir;
    /* 0x1C */ f32 mCutoff;
};

class JPAFieldVortex : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldVortex() {}

    /* 0x10 */ EGG::Vector3f field_0x10;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
};

class JPAFieldRandom : public JPAFieldBase {
public:
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldRandom() {}
};

class JPAFieldDrag : public JPAFieldBase {
public:
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldDrag() {}
};

class JPAFieldConvection : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldConvection() {}

    /* 0x10 */ EGG::Vector3f field_0x10;
    /* 0x1C */ EGG::Vector3f field_0x1c;
    /* 0x28 */ EGG::Vector3f field_0x28;
};

class JPAFieldSpin : public JPAFieldBase {
public:
    void prepare(JPAEmitterWorkData*, JPAFieldBlock*);
    void calc(JPAEmitterWorkData*, JPAFieldBlock*, JPABaseParticle*);
    ~JPAFieldSpin() {}

    /* 0x10 */ EGG::Vector3f field_0x10;
    /* 0x1C */ EGG::Vector3f field_0x1c;
    /* 0x28 */ EGG::Vector3f field_0x28;
};

// unknown name
class JPAFieldBlockData {
public:
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ EGG::Vector3f mPos;
    /* 0x18 */ EGG::Vector3f mDir;
    /* 0x24 */ f32 mMag;
    /* 0x28 */ f32 mMagRndm;
    /* 0x2C */ f32 mVal1;
    /* 0x30 */ f32 mFadeInTime;
    /* 0x34 */ f32 mFadeOutTime;
    /* 0x38 */ f32 mEnTime;
    /* 0x3C */ f32 mDisTime;
    /* 0x40 */ u8 mCycle;
};

class JPAFieldBlock {
public:
    JPAFieldBlock(u8 const*, EGG::Heap*);
    void init(EGG::Heap*);

    u32 getType() { return mpData->mFlags & 0xF; }
    u32 getAddType() { return (mpData->mFlags >> 8) & 3; }
    u32 getSttFlag() { return mpData->mFlags >> 16; }
    bool checkStatus(u16 flag) { return flag & getSttFlag(); }
    f32 getMagRndm() const { return mpData->mMagRndm; }
    f32 getVal1() const { return mpData->mVal1; }
    f32 getFadeInTime() { return mpData->mFadeInTime; }
    f32 getFadeOutTime() { return mpData->mFadeOutTime; }
    f32 getEnTime() { return mpData->mEnTime; }
    f32 getDisTime() { return mpData->mDisTime; }
    u8 getCycle() { return mpData->mCycle; }
    f32 getFadeInRate() { return mFadeInRate; }
    f32 getFadeOutRate() { return mFadeOutRate; }
    EGG::Vector3f& getPos() { return mPos; }
    EGG::Vector3f& getDir() { return mDir; }
    f32 getMag() const { return mMag; }
    void getPosOrig(EGG::Vector3f* pos) { pos->set(mpData->mPos); }
    void getDirOrig(EGG::Vector3f* dir) { dir->set(mpData->mDir); }
    f32 getMagOrig() { return mpData->mMag; }
    void initOpParam() {
        getPosOrig(&mPos);
        getDirOrig(&mDir);
        mMag = getMagOrig();
    }
    void prepare(JPAEmitterWorkData* work) { mpField->prepare(work, this); }
    void calc(JPAEmitterWorkData* work, JPABaseParticle* ptcl) { mpField->calc(work, this, ptcl); }

private:
    /* 0x00 */ const JPAFieldBlockData* mpData;
    /* 0x04 */ JPAFieldBase* mpField;
    /* 0x08 */ f32 mFadeInRate;
    /* 0x0C */ f32 mFadeOutRate;
    /* 0x10 */ EGG::Vector3f mPos;
    /* 0x1C */ EGG::Vector3f mDir;
    /* 0x28 */ f32 mMag;

    enum Type {
        /* 0x0 */ FIELD_GRAVITY,
        /* 0x1 */ FIELD_AIR,
        /* 0x2 */ FIELD_MAGNET,
        /* 0x3 */ FIELD_NEWTON,
        /* 0x4 */ FIELD_VORTEX,
        /* 0x5 */ FIELD_RANDOM,
        /* 0x6 */ FIELD_DRAG,
        /* 0x7 */ FIELD_CONVECTION,
        /* 0x8 */ FIELD_SPIN,
    };
};

#endif /* JPAFIELDBLOCK_H */
