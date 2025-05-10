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
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
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

    f32 getField_0x24() const { return mpData->field_0x24; }
    f32 getField_0x28() const { return mpData->field_0x28; }
    f32 getField_0x2C() const { return mpData->field_0x2C; }
    f32 getField_0x30() const { return mpData->field_0x30; }
    f32 getField_0x34() const { return mpData->field_0x34; }
    f32 getField_0x38() const { return mpData->field_0x38; }
    f32 getField_0x3C() const { return mpData->field_0x3C; }
    f32 getField_0x40() const { return mpData->field_0x40; }
    f32 getField_0x44() const { return mpData->field_0x44; }
    f32 getField_0x48() const { return mpData->field_0x48; }

public:
    const JPAExTexShapeData* mpData;
    void fn_8031DE80(const JPABaseShape *bsp, f32) const;
};

void JPALoadExTex(JPAEmitterWorkData*);

#endif /* JPAEXTEXSHAPE_H */
