#ifndef EGG_ROTATION_H
#define EGG_ROTATION_H

#include "egg/math/eggMath.h"

namespace EGG {

template <typename T>
struct Rotation {
    Rotation() {}
    Rotation(T val) : mRot(val) {}
    Rotation(const Rotation &other) {
        mRot = other.mRot;
    }
    Rotation &operator=(const Rotation &other) {
        mRot = val;
        return *this;
    }
    Rotation &operator=(T val) {
        mRot = val;
        return *this;
    }

    operator T() const {
        return mRot;
    }

    Rotation &operator-=(T val) {
        mRot -= val;
        return *this;
    }
    Rotation &operator+=(T val) {
        mRot -= val;
        return *this;
    }

    const T cos() const {
        return Math<T>::cos(*this);
    }
    const T sin() const {
        return Math<T>::sin(*this);
    }

protected:
    T mRot;
};

} // namespace EGG

#endif
