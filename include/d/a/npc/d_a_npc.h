#ifndef D_A_NPC_H
#define D_A_NPC_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_FStateID.hpp"
#include "s/s_FStateMgr.hpp"
#include "s/s_StateID.hpp"
#include "s/s_StateMethodUsr_FI.hpp"

// Virtual states!
template <typename T>
class sVFStateID_c : public sFStateID_c<T> {
public:
    typedef void (T::*stateFunc)();
    sVFStateID_c(sStateID_c *superState, const char *name, stateFunc initialize, stateFunc execute, stateFunc finalize)
        : sFStateID_c<T>(name, initialize, execute, finalize), mpSuperState(superState) {}

    virtual unsigned int number() const {
        return getRootState()->numberBase();
    }

    const sVFStateID_c<T> *getRootState() const {
        if (!mpSuperState->isNull()) {
            return static_cast<const sVFStateID_c<T> *>(mpSuperState)->getRootState();
        }
        return this;
    }

    unsigned int numberBase() const {
        return sStateID_c::number();
    }


private:
    sStateID_c *mpSuperState;
};

#define STATE_VIRTUAL_FUNC_DECLARE(class, name)                                                                        \
    virtual void initializeState_##name();                                                                             \
    virtual void executeState_##name();                                                                                \
    virtual void finalizeState_##name();                                                                               \
    static sVFStateID_c<class> StateID_##name

#define STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(class, super_class, name)                                                  \
    virtual void initializeState_##name() override;                                                                    \
    virtual void executeState_##name() override;                                                                       \
    virtual void finalizeState_##name() override;                                                                      \
    static sVFStateID_c<class> StateID_##name;                                                                         \

#define STATE_VIRTUAL_DEFINE(class, name)                                                                              \
    sStateID_c *getDefaultSuperState_##class##name() {                                                                 \
        return &sStateID::null;                                                                                        \
    }                                                                                                                  \
    sVFStateID_c<class> class ::StateID_##name(                                                                        \
        getDefaultSuperState_##class##name(), #class "::StateID_" #name, &class ::initializeState_##name,              \
        &class ::executeState_##name, &class ::finalizeState_##name                                                    \
    )

#define STATE_VIRTUAL_MGR_DECLARE(class_name) sFStateMgr_c<class_name, sStateMethodUsr_FI_c> mStateMgr;

// This is the NPC base. Most npcs actually use dAcOrdinaryNpc, but this just is a simpler one?

// Ghidra: ActorNpcBase
//   size: 0x6e4
// official name
class dAcNpc_c : public dAcObjBase_c {
public:
    dAcNpc_c();

    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Demo);

    STATE_VIRTUAL_MGR_DECLARE(dAcNpc_c);
};

#endif
