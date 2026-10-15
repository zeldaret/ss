#ifndef D_PASTPRESENT_H
#define D_PASTPRESENT_H

#include "common.h"
#include "m/m_vec.h"

class dPastPresent_c {
public:
    dPastPresent_c() : mPast(false), mPresent(false) {}

    void update(s32 roomId, const mVec3_c &pos, f32 f);

    bool isPast() const {
        return mPast || !mPresent;
    }
    bool isPresent() const {
        return !mPast || mPresent;
    }

private:
    bool mPast;
    bool mPresent;
};

#endif
