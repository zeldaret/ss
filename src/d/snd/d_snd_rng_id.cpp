#include "d/snd/d_snd_rng_id.h"

dSndRngId_c::dSndRngId_c() : mIsConfigured(false) {}

void dSndRngId_c::configure(u32 start, u32 end, u8 genChance) {
    mBaseId = start;
    mLastPicked = -1;
    mPickedMask = 0;
    mPickedNum = 0;
    mRange = end - start + 1;
    mGenChance = genChance;
    mIsConfigured = true;
}


u32 dSndRngId_c::rndIdNoReuse() {
    if (mPickedNum >= mRange) {
        return -1;
    }

    if (!rndBool(mGenChance)) {
        return -1;
    }

    u32 result = mLastPicked;

    if (mRange == 1) {
        // only one possible ID
        return mBaseId;
    } else if (mRange == 2) {
        // not alternating like below
        result = mBaseId + rndInt(2);
    } else {
        int remaining = mRange - mPickedNum;
        if (remaining == 1) {
            // find the single last remaining ID
            for (int i = 0; i < mRange; i++) {
                if (((1 << i) & ~mPickedMask) != 0) {
                    result = mBaseId + i;
                }
            }
            
        } else {
            while (mLastPicked == result) {
                // pick a remaining ID and find it
                int pick = rndInt(remaining);
                int j = 0;
                for (int i = 0; i < mRange; i++) {
                    if (((1 << i) & ~mPickedMask) != 0) {
                        if (j == pick) {
                            result = mBaseId + i;
                        }
                        j++;
                    }
                }
            }
        }
    }

    return result;
}

u32 dSndRngId_c::nextIdNoReuse() {
    if (mPickedNum >= mRange) {
        return -1;
    }

    if (!rndBool(mGenChance)) {
        return -1;
    }

    if (mRange == 1) {
        // only one possible ID
        return mBaseId;
    }

    for (int i = 0; i < mRange; i++) {
        if (((1 << i) & ~mPickedMask) != 0) {
            return mBaseId + i;
        }
    }

    return -1;
}

u32 dSndRngId_c::rndId() {
    if (!rndBool(mGenChance)) {
        return -1;
    }
    return mBaseId + rndInt(mRange);
}

u32 dSndRngId_c::rndIdNotSame() {
    if (!rndBool(mGenChance)) {
        return -1;
    }

    u32 last = mLastPicked;
    if (mRange == 1) {
        // we have to reuse if we only have one ID to pick from
        return mBaseId;
    }

    u32 result = mLastPicked;

    if (mRange == 2) {
        if (last == -1) {
            // we haven't generated an ID yet, so pick one of the two at random
            result = mBaseId + rndInt(2);
        } else {
            // we have generated an ID before, so pick the other one now
            if (last == mBaseId) {
                result = mBaseId + 1;
            } else {
                result = mBaseId;
            }
        }
    } else {
        while (mLastPicked == result) {
            result = mBaseId + rndInt(mRange);
        }
    }
    

    return result;
}

void dSndRngId_c::markPicked(u32 id, bool allowReset) {
    if (id != -1 && id >= mBaseId && id < mBaseId + mRange) {
        if (mRange >= 2) {
            u32 bit = 1 << (id - mBaseId);
            if (!(mPickedMask & bit)) {
                mPickedMask |= bit;
                mPickedNum++;
            }

            if (allowReset && mPickedNum >= mRange) {
                resetPicked();
            }
            mLastPicked = id;
        }
    }
}

void dSndRngId_c::resetPicked() {
    mPickedMask = 0;
    mPickedNum = 0;
}

u32 dSndRngId_c::rndInt(s32 max) {
    return dSndRng_c::rndInt(max);
}
