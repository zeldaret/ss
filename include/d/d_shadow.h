#ifndef D_SHADOW_H
#define D_SHADOW_H

#include "d/d_light_env.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_sphere.h"
#include "rvl/GX/GXTexture.h"

class dAcObjBase_c;

// This is unofficial, idk if this is close, but this is a common shadow extension.
class dShadowCircle_c : public m3d::mShadowCircle_c {
public:
    dShadowCircle_c() {}
    virtual ~dShadowCircle_c() {}

    bool drawMdl(
        m3d::smdl_c &mdl, const mSphere_c &sph, ActorLighting &actorLighting, u32 priority, s32 alpha, dAcObjBase_c *,
        f32 height
    );
    bool drawTexObj(
        const GXTexObj *pTex, const mMtx_c &mtx, const mSphere_c &sph, ActorLighting &actorLighting, u32 priority,
        s32 alpha, s32 red, s32 green, s32 blue, dAcObjBase_c *, f32 height
    );
};

#endif
