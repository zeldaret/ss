#ifndef RAII_PTR_H
#define RAII_PTR_H

#include "common.h"

// This could be std::unique_ptr, but we don't have it yet
template <typename T>
class RaiiPtr {
public:
    T *mPtr;

    RaiiPtr() : mPtr(nullptr) {}
    ~RaiiPtr() {
        if (mPtr != nullptr) {
            delete mPtr;
            mPtr = nullptr;
        }
    }

    void operator=(T *ptr) {
        mPtr = ptr;
    }

    operator bool() const {
        return mPtr != nullptr;
    }

    const T *get() const {
        return mPtr;
    }

    T *get() {
        return mPtr;
    }

    const T *operator->() const {
        return mPtr;
    }

    T *operator->() {
        return mPtr;
    }

    const T &operator*() const {
        return *this->operator->();
    }

    T &operator*() {
        return *this->operator->();
    }
};

#endif
