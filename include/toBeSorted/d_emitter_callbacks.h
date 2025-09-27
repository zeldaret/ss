#ifndef D_EMITTER_CALLBACKS_H
#define D_EMITTER_CALLBACKS_H

#include "m/m_vec.h"
#include "toBeSorted/d_emitter.h"

/**
 * A callback responsible for emitting the trail left by the red item cursor dot.
 * Used by Bow, Gust Bellows, Clawshots, Slingshot.
 */
class dEmitterCallbackCursorTrail_c : public dEmitterCallback_c {
public:
    dEmitterCallbackCursorTrail_c(u32 x = 0x28, f32 y = 3.5f);
    virtual ~dEmitterCallbackCursorTrail_c() {}

    virtual void executeAfter(JPABaseEmitter *) override;
    virtual void create(JPABaseEmitter *) override;
    virtual void vt_0x20(f32, f32) override;

private:
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u8 _0x16[0x1C - 0x16];
    /* 0x1C */ mVec3_c posNMinus1;
    /* 0x28 */ mVec3_c posNMinus2;
};

#endif
