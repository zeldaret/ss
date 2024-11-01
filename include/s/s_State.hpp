#ifndef S_STATE_H
#define S_STATE_H

#include "s/s_FStateMgr.hpp"
#include "s/s_StateMethodUsr_FI.hpp"

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

#endif
