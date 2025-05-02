#ifndef D_PARTICLE_H
#define D_PARTICLE_H

#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

#define D_PARTICLE_RES_COMMON 0
#define D_PARTICLE_RES_PARTICLE 1

#define D_PARTICLE_NUM_RES 2

namespace dParticle {

struct JPN {
    u16 numJpn;
    u16 mData[1];
};

class res_c {
public:
    res_c() {
        mpHeap = nullptr;
        mpResourceManager = nullptr;
        mpJpnData = nullptr;
    }

    JPAResourceManager *init(const void *jpc, const void *jpn, EGG::Heap *heap);
    void dispose();

    JPAResourceManager *getResourceManager() {
        return mpResourceManager;
    }

    const JPN *getJpn() const {
        return mpJpnData;
    }

private:
    /* 0x00 */ EGG::FrmHeap *mpHeap;
    /* 0x04 */ JPAResourceManager *mpResourceManager;
    /* 0x08 */ const JPN *mpJpnData;
};

// Made up name
class tex_c {
public:
    tex_c() {
        field_0x04 = 0;
        field_0x08 = 0;
    }
    virtual void set(EGG::CpuTexture *tex);

private:
    /* 0x04 */ UNKWORD field_0x04;
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ EGG::ResTIMG mImg;
};

// Made up name
class mgr_c {
public:
    mgr_c(EGG::Heap *heap, u32 ptclMax, u32 emtrMax, u8 grpMax);

    static mgr_c *GetInstance() {
        return sInstance;
    }

    static mgr_c *create(EGG::Heap *heap, u32 ptclMax, u32 emtrMax, u8 grpMax);

    void createResource(EGG::Heap *heap, s32 mgrIndex, void *jpc, void *jpn);
    void disposeResource(s32 mgrIndex);
    s32 getResUserWork(u16 handle) const;
    u16 getJpnData(u16 handle) const;
    // Invokes the JParticle 11 extension
    void doCustomSkywardSwordThing(u8 groupId, f32 x, f32 y);
    void calc(u8 groupId);
    void draw(const JPADrawInfo *drawInfo, u8 groupId);
    JPABaseEmitter *
    createEmitter(u16 particleResId, u8 groupId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale);
    void forceDeleteAllEmitter();
    void forceDeleteEmitter(JPABaseEmitter *emitter);
    JPAResourceManager *getResourceManager(s32 idx);

private:
    static mgr_c *sInstance;
    /* 0x00 */ JPAEmitterManager *mpEmitterMgr;
    /* 0x04 */ res_c mRes[D_PARTICLE_NUM_RES];
};

} // namespace dParticle

#endif
