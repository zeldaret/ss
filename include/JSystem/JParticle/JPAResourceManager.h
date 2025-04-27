#ifndef JPARESOURCEMANAGER_H
#define JPARESOURCEMANAGER_H

#include "common.h"
#include "egg/egg_types.h"
#include "rvl/GX.h"
#include "JSystem/JParticle/JPATexture.h"

class JPAResource;

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAResourceManager {
public:
    JPAResourceManager(void const*, EGG::Heap*);
    JPAResource* getResource(u16) const;
    bool checkUserIndexDuplication(u16) const;
    const EGG::ResTIMG* swapTexture(EGG::ResTIMG const*, char const*);
    void registRes(JPAResource*);
    void registTex(JPATexture*);
    u32 getResUserWork(u16) const;

    void load(u16 idx, GXTexMapID texMapID) { mpTexArr[idx]->load(texMapID); }

public:
    /* 0x00 */ EGG::Heap* mpHeap;
    /* 0x04 */ JPAResource** mpResArr;
    /* 0x08 */ JPATexture** mpTexArr;
    /* 0x0C */ u16 mResMax;
    /* 0x0E */ u16 mResNum;
    /* 0x10 */ u16 mTexMax;
    /* 0x12 */ u16 mTexNum;
};

#endif /* JPARESOURCEMANAGER_H */
