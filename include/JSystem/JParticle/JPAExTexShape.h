#ifndef JPAEXTEXSHAPE_H
#define JPAEXTEXSHAPE_H

#include "JSystem/JParticle/JPABaseShape.h"
#include "common.h"

class JPAEmitterWorkData;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAExTexShapeData {
    // Common header.
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;

    /* 0x08 */ u32 mFlags;
    /* 0x0C */ f32 mIndTexMtx[2][3];
    /* 0x24 */ u8 _0x24[0x4C - 0x24];
    /* 0x4C */ u8 field_0x4C;
    /* 0x4D */ s8 mExpScale;
    /* 0x4E */ s8 mIndTexIdx;
    /* 0x4F */ s8 mSecTexIdx;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAExTexShape {
public:
    JPAExTexShape(u8 const*);

    const f32* getIndTexMtx() const { return &mpData->mIndTexMtx[0][0]; }
    u8 getfield_0x4C() const { return mpData->field_0x4C; }
    s8 getExpScale() const { return mpData->mExpScale; }
    u8 getIndTexIdx() const { return mpData->mIndTexIdx; }
    u8 getSecTexIdx() const { return mpData->mSecTexIdx; }
    bool isUseIndirect() const { return !!(mpData->mFlags & 0x01); }
    BOOL isUseSecTex() const { return (mpData->mFlags & 0x0100); }

public:
    const JPAExTexShapeData* mpData;
    void fn_8031DE80(JPABaseShape *bsp, f32);
};

void JPALoadExTex(JPAEmitterWorkData*);

#endif /* JPAEXTEXSHAPE_H */
