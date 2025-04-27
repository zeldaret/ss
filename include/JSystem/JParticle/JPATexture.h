#ifndef JPATEXTURE_H
#define JPATEXTURE_H

#include "common.h"
#include "rvl/GX.h"
#include "egg/gfx/eggTexture.h"

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPATextureData {
    // Probably magic / size / flags up top here, but they're unused.
    /* 0x00 */ char field_0x00[0x0C];
    /* 0x0C */ char mName[0x14];
    /* 0x20 */ EGG::ResTIMG mResTIMG;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPATexture {
public:
    JPATexture(u8 const*);
    virtual ~JPATexture();

    void load(GXTexMapID texMapID) { mTexture.load(texMapID); }

    EGG::Texture* getJUTTexture() { return &mTexture; }
    const char* getName() const { return mpData->mName; }

public:
    EGG::Texture mTexture;
    const JPATextureData* mpData;
};

#endif /* JPATEXTURE_H */
