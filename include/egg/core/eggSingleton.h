#ifndef EGG_CORE_SINGLETON_H
#define EGG_CORE_SINGLETON_H

#include "egg/core/eggDisposer.h"
#include "egg/prim/eggAssert.h"

/**
 * @brief Declares functions and data for a disposable singleton class
 */
#define EGG_SINGLETON_DECL(T)                                                                                          \
protected:                                                                                                             \
    class T__Disposer : public EGG::Disposer {                                                                         \
    public:                                                                                                            \
        virtual ~T__Disposer(); /* at 0x8 */                                                                           \
        static T__Disposer *sStaticDisposer;                                                                           \
    };                                                                                                                 \
                                                                                                                       \
    T__Disposer mDisposer;                                                                                             \
                                                                                                                       \
public:                                                                                                                \
    static T *createInstance();                                                                                        \
    static void deleteInstance();                                                                                      \
                                                                                                                       \
    static T *instance() {                                                                                             \
        return sInstance;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
protected:                                                                                                             \
    static T *sInstance;

/**
 * @brief Defines functions and data for a disposable singleton class
 */
#define EGG_SINGLETON_IMPL(T)                                                                                          \
    T::T__Disposer::~T__Disposer() {                                                                                   \
        T::deleteInstance();                                                                                           \
    }                                                                                                                  \
                                                                                                                       \
    T *T::createInstance() {                                                                                           \
        if (sInstance == NULL) {                                                                                       \
            sInstance = new T();                                                                                       \
        } else {                                                                                                       \
            EGG_ASSERT_MSG(false, "Create Singleton Twice (%s) : addr %x\n", #T, sInstance);                           \
        }                                                                                                              \
                                                                                                                       \
        return sInstance;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void T::deleteInstance() {                                                                                         \
        sInstance = NULL;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    T *T::sInstance = NULL;

#endif
