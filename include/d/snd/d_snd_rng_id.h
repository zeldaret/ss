#ifndef D_SND_RNG_ID_H
#define D_SND_RNG_ID_H

#include "d/snd/d_snd_rng.h"

/** A class for picking sound ID variants at random without repeats. */
class dSndRngId_c : public dSndRng_c {
public:
    dSndRngId_c();

    void configure(u32 start, u32 end, u8 genChance);

    u32 rndIdNoReuse();
    u32 nextIdNoReuse();
    u32 rndId();
    u32 rndIdNotSame();
    void markPicked(u32 id, bool allowReset);

private:
    void resetPicked();
    u32 rndInt(s32 max);

    /* 0x18 */ u32 mBaseId;
    /* 0x1C */ u32 mLastPicked;
    /* 0x20 */ u32 mPickedMask;
    /* 0x24 */ s16 mPickedNum;
    /* 0x26 */ u8 mRange;
    /* 0x27 */ u8 mGenChance;
    /* 0x28 */ bool mIsConfigured;
};

#endif
