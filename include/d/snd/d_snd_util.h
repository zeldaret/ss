#ifndef D_SND_UTIL_H
#define D_SND_UTIL_H

#include "common.h"
#include "egg/core/eggDisposer.h"

// This setup is only inferred. d/snd uses it all over the place.
// This works for dSndPlayerMgr_c, which has a vtable of its own but the Disposer at offset 0.
// It also works for the factory at 0x80399c20, which calls a base class ctor,
// an inline ctor, and has the Disposer at offset 0x18

template <typename T>
struct SndMgrDisposer : public EGG::Disposer {
    virtual ~SndMgrDisposer();

    static T *create();
    static void remove();
};

template <typename T>
SndMgrDisposer<T>::~SndMgrDisposer() {
    if (this == T::sDisposer) {
        remove();
    }
}

template <typename T>
T *SndMgrDisposer<T>::create() {
    if (T::sInstance == nullptr) {
        T::sInstance = new T();
        T::sDisposer = T::sInstance->GetDisposer();
    }
    return T::sInstance;
}

template <typename T>
void SndMgrDisposer<T>::remove() {
    T::sInstance = nullptr;
    T::sDisposer = nullptr;
}

#define SND_DISPOSER_FORWARD_DECL(class_name)                                                                          \
    class class_name;                                                                                                  \
    extern template class SndMgrDisposer<class_name>;

#define SND_DISPOSER_DEFINE(class_name)                                                                                \
    template class SndMgrDisposer<class_name>;                                                                         \
    class_name *class_name::sInstance;                                                                                 \
    SndMgrDisposer<class_name> *class_name::sDisposer;

#define SND_DISPOSER_MEMBERS(class_name)                                                                               \
public:                                                                                                                \
    SndMgrDisposer<class_name> *GetDisposer() {                                                                        \
        return &mDisposer;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    static class_name *GetInstance() {                                                                                 \
        return sInstance;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    static void create() {                                                                                             \
        SndMgrDisposer<class_name>::create();                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    static class_name *sInstance;                                                                                      \
    static SndMgrDisposer<class_name> *sDisposer;                                                                      \
                                                                                                                       \
private:                                                                                                               \
    SndMgrDisposer<class_name> mDisposer;

#endif
