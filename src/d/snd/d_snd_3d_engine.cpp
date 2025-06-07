#include "d/snd/d_snd_3d_engine.h"

#include "common.h"
#include "nw4r/snd/snd_Sound3DListener.h"

dSnd3DEngine_c::dSnd3DEngine_c() {}

static void CalcPitch(
    const nw4r::math::VEC3 *vec, f32 mag, const nw4r::snd::Sound3DManager &mgr,
    const nw4r::snd::Sound3DListener &listener, const nw4r::snd::Sound3DParam &param, f32 *pPitch
);

void dSnd3DEngine_c::UpdateAmbientParam(
    const nw4r::snd::Sound3DManager *mgr, const nw4r::snd::Sound3DParam *param3d, u32 unk, u32 flags,
    nw4r::snd::SoundAmbientParam *paramAmb
) {
    f32 pitch;
    CalcPitch(nullptr, 0.0f, *mgr, mgr->GetListenerList().GetFront(), *param3d, &pitch);
}

// Largely copied from Sound3DCalculator::CalcPitch
static void CalcPitch(
    const nw4r::math::VEC3 *vec, f32 mag, const nw4r::snd::Sound3DManager &mgr,
    const nw4r::snd::Sound3DListener &listener, const nw4r::snd::Sound3DParam &param, f32 *pPitch
) {
    f32 f0 = mgr.GetField0x24();
    if (f0 == 0.0f) {
        *pPitch = 1.0f;
        return;
    }

    nw4r::math::VEC3 relativePos(*vec);
    if (mag > 0.0f) {
        relativePos /= mag;
    }
    f32 f3 = param.field_0x1E / 32.0f;
    f32 f1, f2;
    if (mag > 0.0f) {
        f1 = -nw4r::math::VEC3Dot(&relativePos, &param.velocity);
        f2 = -nw4r::math::VEC3Dot(&relativePos, &listener.GetVelocity());
    } else {
        f1 = -nw4r::math::VEC3Len(&param.velocity);
        f2 = nw4r::math::VEC3Len(&listener.GetVelocity());
    }
    f32 pitch;
    f1 *= f3;
    f2 *= f3;
    if (f2 > f0) {
        pitch = 0.0f;
    } else if (f1 >= f0) {
        pitch = 65535.0f;
    } else {
        pitch = (f0 - f2) / (f0 - f1);
    }
    *pPitch = pitch;
}
