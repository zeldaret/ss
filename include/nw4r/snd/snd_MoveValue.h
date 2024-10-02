#ifndef NW4R_SND_MOVE_VALUE_H
#define NW4R_SND_MOVE_VALUE_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

template <typename TValue, typename TTime> class MoveValue {
public:
    MoveValue()
        : mOrigin(TValue()),
          mTarget(TValue()),
          mFrame(TTime()),
          mCounter(TTime()) {}

    void InitValue(TValue t1) {
        mOrigin = t1;
        mTarget = t1;
        mFrame = 0;
        mCounter = 0;
    }

    bool IsFinished() const {
        return mCounter >= mFrame;
    }

    TValue GetValue() const {
        if (IsFinished()) {
            return mTarget;
        }

        return mOrigin + mCounter * (mTarget - mOrigin) / mFrame;
    }

    void Update() {
        if (mCounter < mFrame) {
            mCounter++;
        }
    }

    void SetTarget(TValue target, TTime frame) {
        mOrigin = GetValue();
        mTarget = target;
        mFrame = frame;
        mCounter = 0;
    }

private:
    TValue mOrigin; // at 0x0
    TValue mTarget; // at 0x4
    TTime mFrame;   // at 0x8
    TTime mCounter; // at 0xC
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
