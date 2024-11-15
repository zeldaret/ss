#ifndef S_STATE_H
#define S_STATE_H

#include "s/s_FStateMgr.hpp"
#include "s/s_StateMethodUsr_FI.hpp"
#include "s/s_FStateVirtualID.hpp"

// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

#define STATE_FUNC_DECLARE(class, name)                                                                                \
    void initializeState_##name();                                                                                     \
    void executeState_##name();                                                                                        \
    void finalizeState_##name();                                                                                       \
    static sFStateID_c<class> StateID_##name

#define STATE_DEFINE(class, name)                                                                                      \
    sFStateID_c<class> class ::StateID_##name(                                                                         \
        #class "::StateID_" #name, &class ::initializeState_##name, &class ::executeState_##name,                      \
        &class ::finalizeState_##name                                                                                  \
    )

#define STATE_MGR_DECLARE(class_name)                                                                                  \
    sFStateMgr_c<class_name, sStateMethodUsr_FI_c> mStateMgr;                                                          \
    void dummy_GetStateID() {                                                                                          \
        mStateMgr.getStateID();                                                                                        \
    }

#define UI_STATE_MGR_DECLARE(class_name) sFStateMgr_c<class_name, sStateMethodUsr_FI_c> mStateMgr;

#define STATE_MGR(class_name) sFStateMgr_c<class_name, sStateMethodUsr_FI_c>

// TODO this is probably not the whole solution.
// The problems with this approach are:
// * You can't define the same state name for multiple files in the same TU due to baseID_ symbol clash.
// * The use of the templated baseID_ function isn't quite justified (could just use normal functions).

#define STATE_VIRTUAL_FUNC_DECLARE(class, name)                                                                        \
    virtual void initializeState_##name();                                                                             \
    virtual void executeState_##name();                                                                                \
    virtual void finalizeState_##name();                                                                               \
    static const sFStateVirtualID_c<class> StateID_##name;                                                             \
    typedef sStateID_c StateID_##name##_BaseIDClass

#define STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(class, super_class, name)                                                  \
    virtual void initializeState_##name() override;                                                                    \
    virtual void executeState_##name() override;                                                                       \
    virtual void finalizeState_##name() override;                                                                      \
    static const sFStateVirtualID_c<class> StateID_##name;                                                             \
    typedef super_class StateID_##name##_BaseIDClass

#define STATE_VIRTUAL_DEFINE(class, name)                                                                              \
    template <typename T>                                                                                              \
    static const sStateID_c &baseID_##name() {                                                                         \
        return T::StateID_##name;                                                                                      \
    }                                                                                                                  \
    template <>                                                                                                        \
    const sStateID_c &baseID_##name<sStateID_c>() {                                                                    \
        return sStateID::null;                                                                                         \
    }                                                                                                                  \
    const sFStateVirtualID_c<class> class ::StateID_##name(                                                            \
        &baseID_##name<class ::StateID_##name##_BaseIDClass>(), #class "::StateID_" #name,                             \
        &class ::initializeState_##name, &class ::executeState_##name, &class ::finalizeState_##name                   \
    )

#endif
