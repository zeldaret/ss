#include "toBeSorted/d_emitter_callbacks.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "common.h"
#include "m/m_vec.h"

dEmitterCallbackCursorTrail_c::dEmitterCallbackCursorTrail_c(u32 a, f32 b) : field_0x10(b), field_0x14(a) {
    // TODO - TList
}

static void fn_8002B9D0(mVec3_c &newPos, const mVec3_c&, const mVec3_c&, const mVec3_c&, const mVec3_c&, f32) {
    // TODO - ...
    // Probably cubic B-Spline interpolation?
}

void dEmitterCallbackCursorTrail_c::executeAfter(JPABaseEmitter *emitter) {
    mVec3_c newPos;
    mVec3_c pos;
    mVec3_c v2;
    mVec3_c v1;

    emitter->getGlobalTranslation(pos);
    
    if (!emitter->checkStatus(JPAEmtrStts_StopCalc) && emitter->mMaxFrame == 0) {
        v1 = (pos - posNMinus1) * 0.5f;
        v2 = (posNMinus1 - posNMinus2) * 0.5f;
        f32 dist = pos.distance(posNMinus1);
        dist = dist * 0.1f * field_0x10;
        if (field_0x14 != 0 && dist > field_0x14) {
            dist = field_0x14;
        }

        if (dist > 1.0f) {
            f32 f, step;
            step = 1.0f / dist;
            for (f = step; f < 1.0f; f += step) {
                fn_8002B9D0(newPos, posNMinus1, v2, v1, pos, f);
                JPABaseParticle *ptc = emitter->createParticle();
                if (ptc != nullptr) {
                    ptc->setOffsetPosition(newPos);
                }
            }
        }
    }

    if (emitter->getUserWork() == 0) {
        posNMinus2 = posNMinus1;
        posNMinus1 = pos;
    }
}

void dEmitterCallbackCursorTrail_c::create(JPABaseEmitter *emitter) {
    emitter->getGlobalTranslation(posNMinus1);
    posNMinus2 = posNMinus1;
}

void dEmitterCallbackCursorTrail_c::vt_0x20(f32 x, f32 y) {
    posNMinus2.x += x;
    posNMinus2.z += y;
    posNMinus1.x += x;
    posNMinus1.z += y;
}
